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

void user_proc() {
    int pid;
    int start_time;
    int time;
    int sleep_sec;
    char name[PROC_NAME_LEN];
    int rc;

    sp_memset(&name, 0, sizeof(name));

    pid        = get_proc_pid();
    sleep_sec  = (pid * 2) % 10 + 1;
    start_time = get_sys_time();

    rc = get_proc_name(name);

    if (rc != 0) {
        cons_printf("time=%04d pid=%02d ERROR - unable to get the process name; exiting\n");
        proc_exit();
    }

    cons_printf("time=%04d pid=%02d %s started\n", start_time, pid, name);

    while (1) {
        time = get_sys_time();

        if (time - start_time >= 20) {
            cons_printf("time=%04d pid=%02d %s exiting\n", time, pid, name);
            proc_exit();
        }

        if (sleep_sec > 0) {
            cons_printf("time=%04d pid=%02d %s sleeping for %d seconds\n", time, pid, name, sleep_sec);
            sleep(sleep_sec);
        }
    }
}
