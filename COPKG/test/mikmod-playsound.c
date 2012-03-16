/* Modified sample program from mikmod documentation */

/* The bb.s3m file we use to test was taken from the bb demo - http://aa-project.sourceforge.net/bb/ */

#include <windows.h>
#include <stdio.h>
#include <mikmod.h>

static void usage(void)
{
	printf("usage: mikmod-playsound.exe [-t] filename\n");
}

int main(int argc, char *argv[])
{
    MODULE *module;
	char *filename;
	int test;

	if (!argv[1])
	{
		usage();
		return 1;
	}

	if (!strcmp(argv[1], "-t"))
	{
		/* In test mode, mute and quit after 100 ms. */
		test = 1;
		filename = argv[2];
	}
	else
	{
		test = 0;
		filename = argv[1];
	}
	
	if (!filename)
	{
		usage();
		return 1;
	}

    /* register all the drivers */
    MikMod_RegisterAllDrivers();

    /* register all the module loaders */
    MikMod_RegisterAllLoaders();

    /* initialize the library */
    md_mode |= DMODE_SOFT_MUSIC;
    if (MikMod_Init("")) {
        fprintf(stderr, "Could not initialize sound, reason: %s\n",
                MikMod_strerror(MikMod_errno));
        return;
    }

	if (test)
		Player_SetVolume(0);
	
    /* load module */
    module = Player_Load(filename, 64, 0);
    if (module) {
		
        /* start module */
        Player_Start(module);

        while (Player_Active()) {
            /* we're playing */
            Sleep(100);
            MikMod_Update();
			if (test)
				break;
        }

        Player_Stop();
        Player_Free(module);
    } else
        fprintf(stderr, "Could not load module, reason: %s\n",
                MikMod_strerror(MikMod_errno));

	/* Just so we know everything is OK when testing */
	printf("Finished playing %s\n", filename);

    /* give up */
    MikMod_Exit();

	return 0;
}
