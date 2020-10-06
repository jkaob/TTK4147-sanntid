# TASK A
Works as expected. LED0 is 200ms, LED1 is 500ms.
```
static void taskFn(void* args){
	const portTickType delay = (int)args / portTICK_RATE_MS;
	int LED = LED0_GPIO;
    int iter = 0;
	if ((int)args == 500) {
		LED = LED1_GPIO;
	}
	while(1){
		gpio_toggle_pin(LED);
		printf("tick %d\n", iter++);
		
		vTaskDelay(delay);
	}
}

```

# TASK B

vTaskDelay() allows other tasks to execute while "waiting", meaning that idle time of one tasks is time that can be spent doing another task.\
With busy_delay_short() the task is forced to actively wait, meaning that time spent executing this task is spent being idle, \
and the scheduler can't exploit that the task is waiting, and thus cannot execute other tasks.