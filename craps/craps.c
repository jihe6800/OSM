/**
 * Game of luck: Implementation of the Gamemaster
 *
 * Course: Operating Systems and Multicore Programming - OSM lab
 * assignment 1: game of luck.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h> /* I/O functions: printf() ... */
#include <stdlib.h> /* rand(), srand() */
#include <unistd.h> /* read(), write() calls */
#include <assert.h> /* assert() */
#include <time.h>   /* time() */
#include <signal.h> /* kill(), raise() and SIG???? */

#include <sys/types.h> /* pid */
#include <sys/wait.h> /* waitpid() */

#include "common.h"

int main(int argc, char *argv[])
{
	int i, seed;
	int pfd[2*NUM_PLAYERS][2];

        /* TODO: Use the following variables in the exec system call. Using the
	 * function sprintf and the arg1 variable you can pass the id parameter
	 * to the children
	 */
/*
	char arg0[] = "./shooter";
	char arg1[10];
	char *args[] = {arg0, arg1, NULL};
*/
	/* TODO: initialize the communication with the players */
	for (i = 0; i < 2*NUM_PLAYERS; i++) {
	  pipe(pfd[i]);
	  pfd[i][0] = 3;
	  pfd[i][1] = 4;
	}

	for (i = 0; i < NUM_PLAYERS; i++) {
		/* TODO: spawn the processes that simulate the players */

	  switch(fork()){
	  case -1:
	    perror("Failed to fork.");
	    exit(EXIT_FAILURE);
	  case 0:
	    shooter(i, pfd[i*2][0], pfd[i][1]);
	    close(pfd[i][1]);
	    close(pfd[i*2][0]);
	    exit(EXIT_SUCCESS);
	  default:
	    //waitpid(id);
	    break;
	  }
	  //exit(EXIT_SUCCESS);
	}


	seed = time(NULL);
	for (i = 0; i < NUM_PLAYERS; i++) {
		seed++;
		/* TODO: send the seed to the players */
		write(pfd[i*2][1], &seed, sizeof(int));
	}

	/* TODO: get the dice results from the players, find the winner */
	int scores[NUM_PLAYERS];
	for (i = 0; i < NUM_PLAYERS; i++) {
	  read(pfd[i][0], &scores[i], sizeof(int));
	}

	int highest = scores[0], n, winner_tmp;
	for(n=1 ; n<NUM_PLAYERS ; n++){
	  if(highest<scores[n]){
	    winner_tmp = n;
	    highest = scores[n];
	  }
	}
	winner = winner_tmp;
	printf("master: player %d WINS\n", winner);

	/* TODO: signal the winner */
	//signal(SIGUSR1)

	/* TODO: signal all players the end of game */
	for (i = 0; i < NUM_PLAYERS; i++) {

	}

	for (i = 0; i < NUM_PLAYERS; i++) {
	  wait(NULL);
	}

	printf("master: the game ends\n");

	/* TODO: cleanup resources and exit with success */
	for (i = 0; i < NUM_PLAYERS; i++) {
	  wait(NULL);
	}

	return 0;
}
