#include "pwm.h"

int duty      = 50;
int frequency = 10000;
int enable    = 1;

struct task_struct *task;
int pwm_thread(void *data)
{
    int tusec_On;
	int tusec_Off;

	while(1)
	{
		/* Calculate from frequency and dutycycle the delay-times */
		tusec_On  = (1000000*duty)/(frequency*100);			// Duration of on-cycle
		tusec_Off = (1000000*(100-duty))/(frequency*100);	// Duration of off-cycle

		gpio_set_value(PWM_PIN, 1);
		usleep_range(tusec_On, tusec_On);
		gpio_set_value(PWM_PIN, 0);
		usleep_range(tusec_Off, tusec_Off);

		if (kthread_should_stop()) break;
	}
	return 0;
}

void pwm_thread_init(void)
{
	printk(KERN_INFO "PWM: started thread...");
	task = kthread_run(pwm_thread, NULL, THREAD_NAME);
}

void pwm_thread_exit(void)
{
	printk(KERN_INFO "PWM: stopping thread...");
	kthread_stop(task);
}

void pwmInit(void)  {
	gpio_request(PWM_PIN, "Fanctrl");
	gpio_direction_output(PWM_PIN, 0);
	printk(KERN_ALERT "STARTING PWM: Frequency is %dMHz, and dutycycle is %d percent.\n", frequency, duty);
	pwm_thread_init();
}

void pwmDeinit(void)    {
	gpio_free(PWM_PIN);
}

void pwmSetDuty(int cycle)   {
	duty = cycle;
}
