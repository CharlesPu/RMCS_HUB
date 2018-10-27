# RMCS_HUB
A RMCS needs a hub platform to receive and pre-process data from RTUs. Under this backgroud, a common data transmission hub is been designed, whose some inspirations come from aliyun IoT. Meanwhile, it is part of my master's topic.

---
## software framework

---
## VERSIONS
##### date:         2018-10-19
##### version:      v0.0.2
##### description:
* Created.

##### date:         2018-10-25
##### version:      v0.2.0
##### description:
* Finish the frame of the project.

##### date:         2018-10-27
##### version:      v0.4.0
##### description:
* Change the circle queue from per rtu to only one.
* Drop OpenMP for cir_queue process cause only one cir_queue.
* Drop OpenMP for epoll_wait()'s fd process cause the non-reentrant of "HUB_Server".
* Leave the phase and store part in server.cpp and process.cpp, of course, cmd_send.cpp is empty.