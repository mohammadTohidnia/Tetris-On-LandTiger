#pragma once

#include <functional>
#include <forward_list>
#include <map>
#include <thread>
#include <mutex>
#include <wtypes.h>

#include "CONST.h"

class LTIMPORT KeyListener
{
private:
	static ULONG64 handles;
	static std::once_flag once;
	static std::unique_ptr<KeyListener> instance;

	struct key_s;

	friend BOOL operator==(const struct key_s& a, const struct key_s& b);

	volatile BOOL collapsing;
	std::thread thr;
	std::mutex mutex;
	std::map<UINT, std::forward_list<struct key_s>> map;
	KeyListener();
public:
	static KeyListener* sharedInstance();
	ULONG64 addListenerForKey(UINT key, std::function<void(UINT, BOOL)> handler, BOOL continuously = FALSE);
	void removeListenerForKey(UINT key, ULONG64 handle);
	~KeyListener();
};

#define keyListener KeyListener::sharedInstance()

class LTIMPORT KeyRAI
{
private:
	KeyRAI();
	UINT key;
	ULONG64 handle;
public:
	KeyRAI(UINT key, std::function<void(UINT, BOOL)> handler, BOOL continuously = FALSE);
	~KeyRAI();
};