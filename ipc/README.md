Inter process communication.
============================


## Debugging  

ipcs provides information about the communication facilities.  

% _ipcs -m_


------ Shared Memory Segments --------  
|key|shmid|owner|perms|bytes|nattch|status|  
|---|---|---|---|---|---|  
|0x00000000|1627649|user|640|25600|0|  


If this memory segment was erroneously left behind by a program, you can use the ipcrm command to remove it.  
%_ipcrm shm 1627649_

