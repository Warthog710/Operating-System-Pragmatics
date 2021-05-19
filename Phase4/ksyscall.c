#include "spede.h"
#include "kernel.h"
#include "kproc.h"
#include "kutil.h"
#include "string.h"
#include "queue.h"
#include "ksyscall.h"


// function headers queuein & queueout for mailbox
int mbox_queueIn(msg_t*, int); 
int mbox_queueOut(msg_t*, int); 

/**
 * Adds a message to the tail of the specified mailbox
 * @param  msg      - pointer to the message
 * @param  mbox_num - specified mailbox
 * @return -1 on error; 0 on success
 */

/**
 * System call kernel handler: get_sys_time
 * Returns the current system time (in seconds)
 */
void ksyscall_get_sys_time()
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Copy the system time from the kernel to the
    // eax register via the running process' trapframe
    pcb[active_pid].trapframe_p->ebx = (system_time / CLK_TCK);
}

/**
 * System call kernel handler: get_proc_id
 * Returns the currently running process ID
 */
void ksyscall_get_proc_pid() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Copy the running pid from the kernel to the
    // ebx register via the running process' trapframe
    pcb[active_pid].trapframe_p->ebx = active_pid;
}

/**
 * System call kernel handler: get_proc_name
 * Returns the currently running process name
 */
void ksyscall_get_proc_name() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Set the pointer to the address passed in via EBX
    // Copy the string name from the PCB to the destination
    sp_strncpy((char*)pcb[active_pid].trapframe_p->ebx, pcb[active_pid].name, PROC_NAME_LEN);

    // Indicate success or failure via a return code
    // Set the return code
    if((char*)pcb[active_pid].trapframe_p->ebx == NULL)
    {
        pcb[active_pid].trapframe_p->ecx = -1;
    }
    else
    {
        // return of zero for valid name
        pcb[active_pid].trapframe_p->ecx = 0;
    }
}

/**
 * System call kernel handler: sleep
 * Puts the currently running process to sleep
 */
void ksyscall_sleep() 
{
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // Calculate the wake time for the currently running process
    pcb[active_pid].wake_time = pcb[active_pid].trapframe_p->ebx + (system_time / CLK_TCK);

    // Move the currently running process to the sleep queue
    queue_in(&sleep_q, active_pid);

    // Change the running process state to SLEEPING
    pcb[active_pid].state = SLEEPING;

    // Clear the running PID so the process scheduler will run
    active_pid = -1;
}


/**
 * System call kernel handler: msg_send
 * Puts a sent message into the defined mailbox and wakes up a process if waiting on a message
 */
void ksyscall_msg_send()
{
    // Non-blocking ~ placing a message and immediately returning
    int mbox_num;
    int waiting_pid;
    msg_t *msg_src = NULL;
    msg_t *msg_dest = NULL;

    // check for invalid Process ID
    if(active_pid < 0 || active_pid > PID_MAX)
    {
        panic("Invalid PID");
    }

    // assign mbox number the Process ID
    mbox_num = pcb[active_pid].trapframe_p->ecx;

    // check for valid mailbox ID
    if(mbox_num < 0 || mbox_num > MBOX_MAX)
    {
        panic("Invalid mailbox ID!");
    }

    // receive msg from process control block (pcb) via trapframe
    msg_src = (msg_t*)pcb[active_pid].trapframe_p->ebx;

    // check for a valid message
    if(msg_src == NULL)
    {
        panic("Invalid msg destination in mailbox!");
    }

    // queue in the message into the waiting queue
    mbox_queueIn(msg_src, mbox_num);

    //check for waiting processes
    if(mailboxes[mbox_num].wait_q.size <= 0)
    {
        // No waiting processes
        return;
    }

    // there is a process waiting, get the waiting PID from...
    // ...mailbox wait queue
    if (queue_out(&mailboxes[mbox_num].wait_q, &waiting_pid) <= -1)
        panic("Failed to queue_out waiting PID!\n");

    // Verify the waiting pid is valid
    if (waiting_pid < 0 || waiting_pid > PID_MAX)
        panic("Received invalid PID from mailbox!\n");

    // grab the waiting processes message and assign it to msg_dest
    msg_dest = (msg_t*)pcb[waiting_pid].trapframe_p->ebx;

    if (msg_dest == NULL)
        panic("Invalid msg_dest!\n");

    // queue out the process in the waiting using mbox_queueOut
    if (mbox_queueOut(msg_dest, mbox_num) <= -1)
        panic("Failed to queue_out msg from mailbox!\n");

    // "Wake up" process by moving it to the Running queue
    if (queue_in(&run_q, waiting_pid) <= -1)
        panic("Failed to queue_in process to run_q!\n");

    // set the process state to RUNNING
    pcb[waiting_pid].state = RUNNING;
}

/**
 * System call kernel handler: msg_recv
 * Reads a message from the defined mailbox. If no message is available the process is blocked
 */
void ksyscall_msg_recv()
{
    int mbox_num;
    msg_t *msg_dest = NULL;

    // check for invalid Process ID
    if(active_pid < 0 || active_pid > PID_MAX)
    {
        panic("Invalid PID");
    }

    // receive msg from process control block (pcb) via trapframe
    msg_dest = (msg_t*)pcb[active_pid].trapframe_p->ebx;

    // assign mbox number the Process ID
    mbox_num = pcb[active_pid].trapframe_p->ecx;

    // check for valid mailbox ID
    if(mbox_num < 0 || mbox_num > MBOX_MAX)
    {
        panic("Invalid mailbox ID!");
    }

    // check for a valid message destination
    if(msg_dest == NULL)
    {
        panic("Invalid msg destination in mailbox!");
    }

    if(mailboxes[mbox_num].size > 0)
    {
        // There is a message to receive
        if(mbox_queueOut(msg_dest, mbox_num) != 0)
        {
            panic("Unable to queue out message from mailbox!");
        }
    }
    else
    {
        // No message to receive from mailbox
        if(queue_in(&mailboxes[mbox_num].wait_q, active_pid) != 0)
        {
            panic("Unable to queue in process to wait queue!");
        }

        // set the running process' state to WAITING
        pcb[active_pid].state = WAITING;

        // clear the running process ID so an other process will be scheduled
        active_pid = -1;
    }
}

//Exits the currently running process
void ksyscall_proc_exit()
{       
    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");
    // Trigger the process to exit
    kproc_exit(active_pid);
}

//Queues in a message to the defined mailbox
//? Also sets msg.sender and msg.time_sent
int mbox_queueIn(msg_t *msg, int mbox_num)
{
    mailbox_t *mb;

    // check for invalid msg ID
    if(mbox_num < 0 || mbox_num > MBOX_MAX)
    {
        panic("\nInvalid mailbox ID!\n");
    }

    // check for an invalid msg
    if(msg == NULL)
    {
        panic("\nInvalid message!\n");
    }

    mb = &mailboxes[mbox_num];

    // Return error if queue is full
    if(mb->size >= MBOX_MAX)
    {
        return -1;
    }

    // copy msg to the tail of the queue
    if (sp_memcpy(&mb->messages[mb->tail], msg, sizeof(msg_t)) == NULL)
        panic("Failed to copy msg into mailbox\n");

    // Set time_sent and sender pid
    mb->messages[mb->tail].sender = active_pid;
    mb->messages[mb->tail].time_sent = system_time / CLK_TCK;

    // move the tail forward
    // If we are at the end of the array, move the tail to the beginning
    mb->tail = (mb->tail + 1) % MBOX_SIZE;

    // increment the size of the mailbox
    mb->size++;

    // Success
    return 0;    
}

//Queues out a message from a defined mailbox
//? Also sets the msg.time_received
int mbox_queueOut(msg_t *msg, int mbox_num)
{
    mailbox_t *mb;

    // check for invalid msg ID
    if(mbox_num < 0 || mbox_num > MBOX_MAX)
    {
        panic("\nInvalid mailbox ID!\n");
    }

    // check for an invalid msg
    if(msg == NULL)
    {
        panic("Invalid message!");
    }

    // assign new mailbox with the pointer to the global mailbox
    mb = &mailboxes[mbox_num];

    // check if mailbox is empty
    if(mb->size <= 0)
    {
        return -1;
    }

    // copy the message to the destination from the mailbox buffer
    if (sp_memcpy(msg, &mb->messages[mb->head], sizeof(msg_t)) == NULL)
        panic("Failed to copy msg from mailbox\n");

    // move the head forward
    mb->head = (mb->head + 1) % MBOX_SIZE;

    // decrement the size
    mb->size--;

    // set msg receive time
    msg->time_received = system_time / CLK_TCK;

    // Success
    return 0;
}

//Initlizes a semaphore
void ksyscall_sem_init() 
{
    sem_t *sem_ptr;

    // If the active_pid is invalid, panic
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // If we recieve a null ptr, panic
    if ((sem_t*)pcb[active_pid].trapframe_p->ebx == NULL)
        panic("Received NULL ptr in sem_init()\n");

    // Grab the address sent
    sem_ptr = (sem_t*)pcb[active_pid].trapframe_p->ebx;

    // If the value is in range of a semaphore index
    if (*sem_ptr >= 0 && *sem_ptr < SEMAPHORE_MAX)
    {
        // If it is already intilized, return
        if (semaphores[*sem_ptr].init == SEMAPHORE_INITIALIZED)
            return; 
    }

    // Else, the semahore needs to be intilized
    if (queue_out(&semaphore_q, sem_ptr) <= -1)
        panic("Failed to queue_out from the semaphore_q!\n");

    if (*sem_ptr < 0 || *sem_ptr > SEMAPHORE_MAX)
        panic("Received an invalid semaphore index!\n");

    semaphores[*sem_ptr].count  = 1;
    semaphores[*sem_ptr].init   = SEMAPHORE_INITIALIZED;
    queue_init(&semaphores[*sem_ptr].wait_q);
}

//Posts a semaphore
void ksyscall_sem_post()
{
    int waiting_pid;
    sem_t sem_index;

    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // If we received a NULL ptr panic
    if ((sem_t*)pcb[active_pid].trapframe_p->ebx == NULL)
        panic("Received NULL ptr in sem_post()\n");

    sem_index = *(sem_t*)pcb[active_pid].trapframe_p->ebx;

    // If the semaphore is in range, and unitilized... panic
    if (sem_index >= 0 && sem_index < SEMAPHORE_MAX)
    {
        if (semaphores[sem_index].init == SEMAPHORE_UNINITIALIZED)
            panic("sem_post() was passed an unitilized semaphore!\n");
    }

    // If -1 is returned, no process was waiting, increment the sem
    if (queue_out(&semaphores[sem_index].wait_q, &waiting_pid) <= -1)
        semaphores[sem_index].count += 1;

    // Else, move the waiting process to the running queue
    else
    {
        // Verify we have an actual process
        if (active_pid < 0 || active_pid > PID_MAX)
            panic("Invalid PID!\n");
        else
        {
            pcb[waiting_pid].state = RUNNING;

            if (queue_in(&run_q, waiting_pid) <= -1)
                panic("Failed to queue_in into the running queue!\n");
        }
    }
}

//Waits on a semaphore
void ksyscall_sem_wait()
{
    sem_t sem_index;

    // Don't do anything if the running PID is invalid
    if (active_pid < 0 || active_pid > PID_MAX)
        panic("Invalid PID!\n");

    // If we received a NULL ptr panic
    if ((sem_t*)pcb[active_pid].trapframe_p->ebx == NULL)
        panic("Received NULL ptr in sem_post()\n");

    sem_index = *(sem_t*)pcb[active_pid].trapframe_p->ebx;

    // If the semaphore is in range, and unitilized... panic
    if (sem_index >= 0 && sem_index < SEMAPHORE_MAX)
    {
        if (semaphores[sem_index].init == SEMAPHORE_UNINITIALIZED)
            panic("sem_post() was passed an unitilized semaphore!\n");
    }

    // If the value of the sem is > 0, decrement
    if (semaphores[sem_index].count > 0)
        semaphores[sem_index].count -= 1;
    else
    {
        if (queue_in(&semaphores[sem_index].wait_q, active_pid) <= -1)
            panic("Failed to queue_in into semaphore wait_q!\n");

        pcb[active_pid].state = WAITING;
        // Clear the running PID so the scheduler will run
        active_pid = -1;
    }
}
