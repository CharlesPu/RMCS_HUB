#ifndef __RMCS_HUB_PROCESS_H__
#define __RMCS_HUB_PROCESS_H__

class HUB_Process
{
public:
	static HUB_Process* Singleton();
	static void *Task(void *);

private:
	class SingletonDestructor
	{
	public:
		~SingletonDestructor()
		{
			if(HUB_Process::_instance != NULL)
            {
                delete HUB_Process::_instance;
                HUB_Process::_instance = NULL;
            }
		}
		
	};
	HUB_Process();
	~HUB_Process();
	static HUB_Process* _instance;
	static SingletonDestructor singleton_destructor;
};

#endif