
/*
xtimeout - kill started application after idle timeout
Written in 2013 by <Ahmet Inan> <ainan@mathematik.uni-freiburg.de>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/screensaver.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "usage: %s NUM CMD\n", argv[0]);
		return 1;
	}
	int timeout = atoi(argv[1]);

	pid_t pid = fork();

	if (pid < 0)
		return 1;

	if (0 == pid) {
		argv += 2;
		return execvp(argv[0], argv);
	}

	xcb_connection_t *con = xcb_connect(0, 0);
	xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(con)).data;

	while (1) {
		sleep(1);
		int status;
		if (waitpid(pid, &status, WNOHANG))
			return 0;
		xcb_screensaver_query_info_cookie_t cookie = xcb_screensaver_query_info(con, screen->root);
		xcb_screensaver_query_info_reply_t *info = xcb_screensaver_query_info_reply(con, cookie, 0);
		int idle = info->ms_since_user_input / 1000;
		free(info);
		if (idle >= timeout)
			return kill(pid, 15);
	}
	return 0;
}

