#pragma once

#define WM_Login 0
#define WM_ShowQrPicture 1
#define WM_Logout 2
#define WM_GetFriendList 3
#define WM_ShowChatRecord 4
#define WM_SendTextMessage 5
#define WM_SendFileMessage 6
#define WM_GetInformation 7
#define WM_SendImageMessage 8
#define WM_SetRoomAnnouncement 9
#define WM_DeleteUser 10
#define WM_QuitChatRoom 11
#define WM_AddGroupMember 12
#define WM_SendXmlCard 13
#define WM_ShowChatRoomMembers 14
#define WM_ShowChatRoomMembersDone 15
#define WM_DecryptDatabase 16
#define WM_AddUser 17
#define WM_SetRoomName 18
#define WM_AutoChat 19
#define WM_CancleAutoChat 20
#define WM_AlreadyLogin 21
#define WM_SendAtMsg 22
#define WM_DelRoomMember 23
#define WM_OpenUrl 24
#define WM_SaveFriendList 25

struct UserInfo {
	wchar_t userId[80];
	wchar_t userNumber[80];
	wchar_t userRemark[80];
	wchar_t userNickName[80];
};

struct MessageSend {
	wchar_t userId[40];
	wchar_t content[MAX_PATH];
};

struct MessageReceive
{
	wchar_t type[10];		//消息类型
	wchar_t source[20];		//消息来源
	wchar_t wxid[40];		//微信ID/群ID
	wchar_t msgSender[40];	//消息发送者
	wchar_t content[200];	//消息内容
	BOOL isMoney;	        //是否是收款消息
};