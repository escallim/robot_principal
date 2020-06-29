#ifndef __COMMANDTASK_H__
#define __COMMANDTASK_H__

typedef enum
{
	INIT,
	WAITING,
	PROCESS
} commandTaskState_t;

#define CMD_START 10
#define CMD_STOP 11

#define MODE_AUTO 20
#define MODE_MANUAL 21

#define MOVE_HALT 30
#define MOVE_GO_FORWARD 31
#define MOVE_GO_BACKWARD 32
#define MOVE_TURN_LEFT 33
#define MOVE_TURN_RIGHT 34

#define POS_ASK 40

#endif /* __COMMANDTASK_H__ */
