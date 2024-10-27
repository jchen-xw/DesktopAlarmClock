#ifndef __TIMERPAGE_H_
#define __TIMERPAGE_H_


#define TIME_DISPLAY_X                          32
#define TIME_DISPLAY_Y                          16

#define TIMER_MIN_DISPLAY_X                     32
#define TIMER_MIN_DISPLAY_Y                     16

#define TIMER_SEC_DISPLAY_X                     56
#define TIMER_SEC_DISPLAY_Y                     16

#define TIMER_MSEC_DISPLAY_X                    80
#define TIMER_MSEC_DISPLAY_Y                    16

/* 第一个结果 */
#define TIMER_RESULT_1_X                        40
#define TIMER_RESULT_1_Y                        38

#define RESULT_1_MIN_X                          40
#define RESULT_1_MIN_Y                          38

#define RESULT_1_SEC_X                          58
#define RESULT_1_SEC_Y                          38

#define RESULT_1_MSEC_X                         76
#define RESULT_1_MSEC_Y                         38

/* 第二个结果 */
#define TIMER_RESULT_2_X                        40
#define TIMER_RESULT_2_Y                        52

#define RESULT_2_HOUR_X                         40
#define RESULT_2_HOUR_Y                         52

#define RESULT_2_MIN_X                          58
#define RESULT_2_MIN_Y                          52

#define RESULT_2_SEC_X                          76
#define RESULT_2_SEC_Y                          52


void Timer_Task( void *params );


#endif /* __TIMERPAGE_H_ */
