
int islock;

void lock()
{
    while(islock == 1);
}

void unlock() 
{
    islock = 0;
}

          
