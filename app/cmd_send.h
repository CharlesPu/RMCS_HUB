#ifndef __RMCS_HUB_CMD_SEND_H__
#define __RMCS_HUB_CMD_SEND_H__

class HUB_CMD_Send
{
public:
	static HUB_CMD_Send* Singleton();
	static void *Task(void *);

private:
	class SingletonDestructor
	{
	public:
		~SingletonDestructor()
		{
			if(HUB_CMD_Send::_instance != NULL)
            {
                delete HUB_CMD_Send::_instance;
                HUB_CMD_Send::_instance = NULL;
            }
		}
		
	};
	HUB_CMD_Send();
	~HUB_CMD_Send();
	static HUB_CMD_Send* _instance;
	static SingletonDestructor singleton_destructor;
};

#endif