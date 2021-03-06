#pragma once

#include "UserInfo.hpp"
#include "UserStorage.h"
#include <string>
#include <cstring>
#include <map>
#include <mutex>
#include "VNOCUser.h"

#define NULLPOINT           -1
#define LOGIN_OK            1
#define TEST_FALSE          2
#define ACCOUNT_NULL        3
#define HAS_LOGINED         4

extern std::mutex userSetMutex;

class CUserManage
{

public:
    UserStorage *_us;
    static CUserManage* GetInstance()
    {
        return &_instance;
    }
    //return value: LOGIN_OK(login-success) TEST_FALSE(verification-failure) NULLPOINT(null-pointer)
    //get the user's information if login-success.
    void initial(UserStorage *us)
    {
        _us = us;
    }
    int Authenticate(const char* szUser,const char* pPassword, userinfo* pUserInfo, int nPassLen = 40)
    {
        if (szUser == NULL || strcmp(szUser, "") == 0)
        {
            return TEST_FALSE;
        }

        strncpy(pUserInfo->strUser, szUser, 40);
        VNOCUser vUser;
        vUser.setNickName(std::string(szUser));
        bool usernameInserted = insertOnlineUser(vUser);
        if(usernameInserted == false)
        {
            return HAS_LOGINED;//this user has logined, deny this login request.
        }
        
        return LOGIN_OK;
        if ( !_us->IfUserExist(szUser) )//check the username in the database
        {
            return ACCOUNT_NULL;
        }
        char strPass[41] = {0};	
        _us->GetPassword(szUser, strPass, 40);

        int i = 0; //verify the password
        do
        {
            if (strPass[i] != pPassword[i])
            {
                return TEST_FALSE;
            }
            ++i;
        } while (i < nPassLen);

        if (pUserInfo == 0)
        {
            return NULLPOINT;
        }

        //get the user's information
        _us->GetUserInfo(szUser, pUserInfo);

        return LOGIN_OK;
    }

    bool deleteOnlineUser(const char* szUser)
    {
        std::string temp = szUser;
        userSetMutex.lock();
        bool deleteResult = 0 != onlineUsers.erase(temp);
        userSetMutex.unlock();
        return deleteResult;
    }

    VNOCUser& getOnlineUser(const std::string userName)
    {
        userSetMutex.lock();
        VNOCUser &vUser =  onlineUsers[userName];
        userSetMutex.unlock();
        return vUser;
    }
private:
    static CUserManage _instance;
    std::map<std::string, VNOCUser> onlineUsers;
    bool insertOnlineUser(const VNOCUser &vuser)
    {
        std::string temp = vuser.getNickName();
        bool insertResult = false;
        userSetMutex.lock();
        if(onlineUsers.find(temp) == onlineUsers.end())
        {
            onlineUsers[temp] = vuser;
            insertResult = true;
        }
        userSetMutex.unlock();
        return insertResult;
    }
};