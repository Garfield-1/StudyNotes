/*
 * 加载动态库时提醒加载失败，需要手动在环境变量中添加动态库路径
 * export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:[lib_path]
 */
#include <stdio.h>
#include <libubox/uloop.h>

struct uloop_timeout timeout;	//创建uloop_timeout全局变量
 
int frequency = 5; //每隔5秒超时一次
 
static void timeout_cb(struct uloop_timeout *t)
{
	printf("[%s] uloop test \n", __FUNCTION__);
	
	uloop_timeout_set(t, frequency * 1000);//设置下次的超时时间
}
 
int main()
{	
	uloop_init();			//使用库初始化	
	
	timeout.cb = timeout_cb;
	
	uloop_timeout_set(&timeout, frequency * 1000);//设置下次的超时时间
 
	uloop_run();

}