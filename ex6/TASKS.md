# TASK A
Got output (Y)

# TASK B
Running on all cores gives much better worst case response time, because the tasks can run in parallell and doesn't have to completely wait for each other to finish. The most common response time is equal for both task A and B, but for task B where all processes run on the same core, there are some cases where to response time extends up to 25000 us.

# TASK D

**Without disturbance enabled - fastest reponse?** Fastest average - busy polling. Best worst-case periodic polling.
**Less impacted by additon of disturbance?** Both seem quite equally impacted by the addition of disturbances,
**With disturbance enabled - any have a predictable and consistent worst-case response time?** Periodic polling seems to have a quite consitent worst-case response time of around 5000 us, while busy-polling seems to exceed 25000 us at times.   
With busy-polling the disturbances needed to have a printf in the while loop to avoid overflow. Periodic polling allowed for this printf-statement to be removed, and resulted in even better performance, with worst-case response time as low as 2500 us, with most of the response times being sub 1000 us.