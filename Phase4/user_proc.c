/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * User Processes
 */
#include "global.h"
#include "spede.h"

#include "user_proc.h"
#include "string.h"
#include "syscall.h"
#include "ipc.h"

typedef struct proc_info_t {
    int pid;
    int time_start;
    int time_sleep;
    char name[PROC_NAME_LEN];
} proc_info_t;

/* "Shared" memory */
int shared_mem;

/* Mailbox number to send messages */
int mbox_num = 1;

/* Semaphore */
sem_t sem;

void user_proc() {
    int pid;
    int start_time;
    int time;
    int sleep_sec;
    char name[PROC_NAME_LEN];

    msg_t msg;
    proc_info_t proc_info;

    sp_memset(&name, 0, sizeof(name));
    get_proc_name(name);

    pid        = get_proc_pid();
    sleep_sec  = pid % 5 + 1;
    start_time = get_sys_time();

    // Set the proc_info data structure
    proc_info.pid = pid;
    proc_info.time_start = start_time;
    proc_info.time_sleep = sleep_sec;
    get_proc_name(proc_info.name);

    // Initialize the message
    sp_memset(&msg, 0, sizeof(msg_t));

    // Set the message data for the proc_info_t struct
    sp_memcpy(msg.data, &proc_info, sizeof(proc_info_t));

    cons_printf("time=%04d pid=%02d %s started\n", start_time, pid, name);

    while (1) {
        time = get_sys_time();

        if (time - start_time >= 10) {
            cons_printf("time=%04d pid=%02d %s exiting\n", time, pid, name);
            msg_send(&msg, mbox_num);
            proc_exit();
        }

        // Sleep for a few seconds
        sleep(sleep_sec);
    }
}

void dispatcher_proc() {
    int pid;
    int time;
    char name[PROC_NAME_LEN];

    msg_t msg;
    proc_info_t proc_info;

    sp_memset(&name, 0, sizeof(name));
    get_proc_name(name);

    pid  = get_proc_pid();
    time = get_sys_time();

    sem_init(&sem);

    cons_printf("time=%04d pid=%02d %s started\n", time, pid, name);

    while (1) {
        // Clear out the message data structure
        sp_memset(&msg, 0, sizeof(msg_t));
        sp_memset(&proc_info, 0, sizeof(proc_info_t));

        // Receive a message from the mailbox
        msg_recv(&msg, mbox_num);

        sp_memcpy(&proc_info, msg.data, sizeof(proc_info_t));

        cons_printf("time=%04d pid=%02d %s received msg(sender=%d, sent=%d, received=%d)\n",
                    time, pid, name, msg.sender, msg.time_sent, msg.time_received);
        cons_printf("time=%04d pid=%02d %s received data=(name=%s, start=%d, sleep=%d)\n",
                    time, pid, name, proc_info.name, proc_info.time_start, proc_info.time_sleep);

        // Get the current system time
        time = get_sys_time();

        // Wait for the semaphore to be posted by the printer process
        sem_wait(&sem);

        // Set the shared memory
        shared_mem = proc_info.pid;

        // Post the semaphore so the printer process can access the shared memory
        sem_post(&sem);

        sleep(1);
    }
}

void printer_proc() {
    int pid;
    int time;
    char name[PROC_NAME_LEN];

    int cached_mem = -1;

    sp_memset(&name, 0, sizeof(name));
    get_proc_name(name);

    pid  = get_proc_pid();
    time = get_sys_time();

    sem_init(&sem);

    cons_printf("time=%04d pid=%02d %s started\n", time, pid, name);

    while (1) {
        // Wait for the semaphore to be posted by the dispatcher process
        sem_wait(&sem);
        time = get_sys_time();

        // Only print when we have new data
        if (cached_mem != shared_mem) {
            cons_printf("time=%04d pid=%02d %s read shared memory (last pid=%d)\n",
                         time, pid, name, shared_mem);
            cached_mem = shared_mem;
        }

        // Post the semaphore so the dispatcher process can access the shared memory
        sem_post(&sem);

        // Sleep for one second
        sleep(1);
    }
}
