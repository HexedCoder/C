/** @file bacon.c
*
* @brief Uses Movies.txt by default or another if -f is used to complete the
 * various functions below for various actors or movies
*
* -h/--help print the program usage
 *
* -f/--file "filename" file name to get info, or Movies.txt by default
* -c/--cast "film" YEAR will print the names of all actors in a film.
* -d/--distance "Actor1" "Actor2" displays Actor1->movie->Actor2 relationship
* -r/--roles "actor" will print all films an actor has starred in.
* -m/--movieinfo "film YEAR" will print film's year and genre.
* -s/--stats "actor" will print connectivity of actor.
* -e/--extremes will print highest and lowest connectivity actors.
*
* @par
* COPYRIGHT NOTICE: (c) 2022 Jacob Hitchcox
*/

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int factorial(int num) {
    if (num < 2){
        return 1;
    }

    return num * factorial(num - 1);

}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        return 1;
    }

    int num = atoi(argv[1]);

    printf("%d\n", factorial(num));

}				/* main() */

/*** END OF FILE ***/
