#include "types.h"
#include "user.h"
#include "pstat.h"

void fork_kids() {

	int i;
	int rc;

	for(i = 1; i < 7; i++) {
		rc = fork();
		if(rc == 0) {
			int tmp, sum = 0;

			switch (i) {
				case 1:
					if(settickets(10) < 0)
						exit();
					break;
				case 2:
					if(settickets(30) < 0)
						exit();
					break;
				case 3: 
					if(settickets(60) < 0)
						exit();
					break;
				case 4:
					if(settickets(90) < 0)
						exit();
					break;
				case 5:
					if(settickets(120) < 0)
						exit();
					break;					
				case 6:
					if(settickets(150) < 0)
						exit();
					break;
			}

			for(tmp = 0; ;tmp++) {
				sum += tmp;
			}

			exit();
		}
	}
}

int main(int argc, char *argvv[]) {

	//int tickettest;

	struct pstat * stats;
	stats = (struct pstat *) malloc(sizeof(struct pstat) * 64);
	int i;

	// (settickets(0) == -1) ? printf(1,"fail\n") : printf(1,"pass\n"); //you fucked
	// (settickets(10) == -1) ? printf(1,"fail\n") : printf(1,"pass\n"); //you good
	// (settickets(40) == -1) ? printf(1,"fail\n") : printf(1,"pass\n"); //you good
	// (settickets(38) == -1) ? printf(1,"fail\n") : printf(1,"pass\n"); //you fucked
	settickets(20); //you good
	// (settickets(160) == -1) ? printf(1,"fail\n") : printf(1,"fpass\n"); //you fucked
	//printf(1, "\n");

	int runtime;

	fork_kids();
	printf(1,",10,30,60,90,120,150\n");
	for(runtime = 0; runtime < 119 ;runtime++) {
		getpinfo(stats);
		printf(1,"%d,", runtime);
		for(i = 0; i < 64; i++) {
			if(stats[i].inuse == 1 && (stats[i].pid != 1 && stats[i].pid != 2 && stats[i].pid != 3)) {
				printf(1,"%d,", stats[i].n_schedule);
			}
		}
		printf(1, "\n");
		sleep(100);
	}
	exit();
}
