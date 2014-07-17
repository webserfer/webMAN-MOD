#include <fcntl.h>
#include <sys/file.h>
#include <stdio.h>
#include <string.h>

int sys_fs_mount(char const* deviceName, char const* deviceFileSystem, char const* devicePath, int writeProt)
{
    lv2syscall8(837, (u64) deviceName, (u64) deviceFileSystem, (u64) devicePath, 0, (u64) writeProt, 0, 0, 0 );
    return_to_user_prog(int);
}

int CopyFile(char* path, char* path2)
{
    int ret = 0;
    s32 fd = -1;
    s32 fd2 = -1;
    u64 lenght = 0LL;

    u64 pos = 0ULL;
    u64 readed = 0, writed = 0;

    char *mem = NULL;

    sysFSStat stat;

    ret= sysLv2FsStat(path, &stat);
    if(ret) goto skip;
    lenght = stat.st_size;

    ret = sysLv2FsOpen(path, 0, &fd, S_IRWXU | S_IRWXG | S_IRWXO, NULL, 0);
    if(ret) goto skip;

    ret = sysLv2FsOpen(path2, SYS_O_WRONLY | SYS_O_CREAT | SYS_O_TRUNC, &fd2, 0777, NULL, 0);
    if(ret) {sysLv2FsClose(fd);goto skip;}

    mem = malloc(0x100000);
    if (mem == NULL) return -1;

    while(pos < lenght) {

        readed = lenght - pos; if(readed > 0x100000ULL) readed = 0x100000ULL;
        ret=sysLv2FsRead(fd, mem, readed, &writed);
        if(ret<0) goto skip;
        if(readed != writed) {ret = 0x8001000C; goto skip;}

        ret=sysLv2FsWrite(fd2, mem, readed, &writed);
        if(ret<0) goto skip;
        if(readed != writed) {ret = 0x8001000C; goto skip;}

        pos += readed;
    }

skip:

    if(mem) free(mem);
    if(fd>=0) sysLv2FsClose(fd);
    if(fd2>=0) sysLv2FsClose(fd2);
    if(ret>0) ret = 0;

    return ret;
}

int sys_get_version(u32 *version)
{
    lv2syscall2(8, 0x7000, (u64)version);
    return_to_user_prog(int);
}

int is_cobra(void)
{
    u32 version = 0x99999999;
    if (sys_get_version(&version) < 0)    return 0;
    if (version != 0x99999999)    return 1;
    return 0;
}

int add_mygame()
{
// -2 failed and cannot rename the backup
// -1 failed
//  0 already
//  1 done

	char gameexit[] = {0x0d, 0x0a, 0x09, 0x09, 0x09, 0x3c, 0x51, 0x75, 0x65, 0x72, 0x79, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x3d, 0x22, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x78, 0x2d, 0x78, 0x6d, 0x62, 0x2f, 0x66, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x2d, 0x70, 0x69, 0x78, 0x6d, 0x61, 0x70, 0x22, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x6b, 0x65, 0x79, 0x3d, 0x22, 0x73, 0x65, 0x67, 0x5f, 0x67, 0x61, 0x6d, 0x65, 0x65, 0x78, 0x69, 0x74, 0x22, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x73, 0x72, 0x63, 0x3d, 0x22, 0x73, 0x65, 0x6c, 0x3a, 0x2f, 0x2f, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x2f, 0x69, 0x6e, 0x67, 0x61, 0x6d, 0x65, 0x3f, 0x70, 0x61, 0x74, 0x68, 0x3d, 0x63, 0x61, 0x74, 0x65, 0x67, 0x6f, 0x72, 0x79, 0x5f, 0x67, 0x61, 0x6d, 0x65, 0x2e, 0x78, 0x6d, 0x6c, 0x23, 0x73, 0x65, 0x67, 0x5f, 0x67, 0x61, 0x6d, 0x65, 0x65, 0x78, 0x69, 0x74, 0x26, 0x74, 0x79, 0x70, 0x65, 0x3d, 0x67, 0x61, 0x6d, 0x65, 0x22, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x2f, 0x3e};
	char fb[] = {0x0d, 0x0a, 0x09, 0x09, 0x09, 0x3c, 0x51, 0x75, 0x65, 0x72, 0x79, 0x20, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x3d, 0x22, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x78, 0x2d, 0x78, 0x6d, 0x62, 0x2f, 0x66, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x2d, 0x70, 0x69, 0x78, 0x6d, 0x61, 0x70, 0x22, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x6b, 0x65, 0x79, 0x3d, 0x22, 0x78, 0x6d, 0x62, 0x5f, 0x61, 0x70, 0x70, 0x33, 0x22, 0x20, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x61, 0x74, 0x74, 0x72, 0x3d, 0x22, 0x78, 0x6d, 0x62, 0x5f, 0x61, 0x70, 0x70, 0x33, 0x22, 0x20, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x73, 0x72, 0x63, 0x3d, 0x22, 0x78, 0x6d, 0x62, 0x3a, 0x2f, 0x2f, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x2f, 0x64, 0x65, 0x76, 0x5f, 0x68, 0x64, 0x64, 0x30, 0x2f, 0x78, 0x6d, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x2f, 0x67, 0x61, 0x6d, 0x65, 0x5f, 0x70, 0x6c, 0x75, 0x67, 0x69, 0x6e, 0x2f, 0x66, 0x62, 0x2e, 0x78, 0x6d, 0x6c, 0x23, 0x73, 0x65, 0x67, 0x5f, 0x66, 0x62, 0x22, 0x0d, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x2f, 0x3e};
	FILE* f;
	long size;
	char *cat;
	char cat_path[255];
	size_t result;
	int i, j, pos=0;
	sysFSStat stat;

	//read original cat
	f=fopen("/dev_flash/vsh/resource/explore/xmb/category_game.xml", "r");
	if(f==NULL) return -1;
	fseek (f , 0 , SEEK_END);
	size = ftell (f);
	fseek(f, 0, SEEK_SET);

	cat = (char*) malloc (sizeof(char)*size);
	if (cat == NULL) {free(cat); return -1;}

	result = fread(cat,1,size, f);
	if (result != size) {free (cat); fclose (f); return -1;}
	fclose (f);

	// is fb.xml entry in cat file ?
	if(strstr(cat, "fb.xml")!=NULL) {free(cat); return 0;}

	// search position
	for(i=0; i < size - sizeof(gameexit); i++) {
		pos=i;
		for(j=0; j<sizeof(gameexit); j++) {
			if( cat[i+j] != gameexit[j]) break;
			if(j==(sizeof(gameexit)-1)) goto out_s;
		}
	}
	out_s:

	//write patched cat
	f=fopen("/dev_hdd0/game/UPDWEBMOD/USRDIR/category_game.xml", "w");
	if(f==NULL) {free(cat); return -1;}
	fwrite(cat, 1, pos, f);
	fwrite(fb, 1, sizeof(fb), f);
	fwrite(&cat[pos], 1, size-pos, f);
	fclose(f);


    // set target path for category_game.xml
    strcpy(cat_path, "/dev_blind/vsh/resource/explore/xmb/category_game.xml");
    if(sysLv2FsStat(cat_path, &stat) != 0) {
        strcpy(cat_path, "/dev_habib/vsh/resource/explore/xmb/category_game.xml");
		if(sysLv2FsStat(cat_path, &stat) != 0) {
			strcpy(cat_path, "/dev_rewrite/vsh/resource/explore/xmb/category_game.xml");

			// mount /dev_blind if category_game.xml is not found
			if(sysLv2FsStat(cat_path, &stat) != 0) {
				if(sys_fs_mount("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_blind", 0) == 0) {
					strcpy(cat_path, "/dev_blind/vsh/resource/explore/xmb/category_game.xml");
					if(sysLv2FsStat(cat_path, &stat) != 0) { free(cat); return -1;}
				} else { free(cat); return -1;}
			}
		}
	}

	// rename category_game.xml as category_game.xml.bak
    char cat_path_bak[255];
	strcpy(cat_path_bak, cat_path);
    strcat(cat_path_bak, ".bak");
	sysLv2FsUnlink(cat_path_bak);
    if(sysLv2FsRename(cat_path, cat_path_bak) != 0) {free(cat); return -1;} ;

    // update category_game.xml
    if(CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/category_game.xml", cat_path) != 0) {
		sysLv2FsUnlink(cat_path);
		if(sysLv2FsRename(cat_path_bak, cat_path)) { //restore category_game.xml from category_game.xml.bak
			{lv2syscall3(392, 0x1004, 0xa, 0x1b6);} ///TRIPLE BIP
			free(cat);
			return -2;
		}
    }
   	free(cat);
	return 1;
}

int main()
{
    sysFSStat s;

    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_EN.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_AR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_CN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_DE.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_ES.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_FR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_GR.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_HU.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_HR.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_IN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_JP.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_KR.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_IT.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_NL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_PL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_PT.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_RU.TXT");
    sysLv2FsUnlink("/dev_hdd0/tmp/LANG_TR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_EN.TXT","/dev_hdd0/tmp/LANG_EN.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_AR.TXT","/dev_hdd0/tmp/LANG_AR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_CN.TXT","/dev_hdd0/tmp/LANG_CN.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_DE.TXT","/dev_hdd0/tmp/LANG_DE.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_ES.TXT","/dev_hdd0/tmp/LANG_ES.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_FR.TXT","/dev_hdd0/tmp/LANG_FR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_GR.TXT","/dev_hdd0/tmp/LANG_GR.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_HU.TXT","/dev_hdd0/tmp/LANG_HU.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_HR.TXT","/dev_hdd0/tmp/LANG_HR.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_IN.TXT","/dev_hdd0/tmp/LANG_IN.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_IT.TXT","/dev_hdd0/tmp/LANG_IT.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_JP.TXT","/dev_hdd0/tmp/LANG_JP.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_KR.TXT","/dev_hdd0/tmp/LANG_KR.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_NL.TXT","/dev_hdd0/tmp/LANG_NL.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_PL.TXT","/dev_hdd0/tmp/LANG_PL.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_PT.TXT","/dev_hdd0/tmp/LANG_PT.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_RU.TXT","/dev_hdd0/tmp/LANG_RU.TXT");
    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_TR.TXT","/dev_hdd0/tmp/LANG_TR.TXT");

    if(sysLv2FsStat("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_XX.TXT", &s))
        CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_XX.TXT","/dev_hdd0/tmp/LANG_XX.TXT");

    char ligne[255];
    if(is_cobra()) {
        FILE* f=NULL;
        if(sysLv2FsStat("/dev_hdd0/boot_plugins.txt", &s) == 0) {
            f=fopen("/dev_hdd0/boot_plugins.txt", "r");
            while(fgets(ligne, 255, f) != NULL) {
                if(strstr(ligne,"webftp_server.sprx") != NULL) {
                    strtok(ligne, "\r\n");
                    sysLv2FsUnlink(ligne);
                    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server.sprx",ligne);
                    goto exit;
                }
            }
            fclose(f);
        }
        f=fopen("/dev_hdd0/boot_plugins.txt", "a");
        fputs("\r\n/dev_hdd0/webftp_server.sprx", f);
        fclose(f);
        sysLv2FsUnlink("/dev_hdd0/webftp_server.sprx");
        CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server.sprx", "/dev_hdd0/webftp_server.sprx");
    }
    else {
        FILE* f=NULL;
        if(sysLv2FsStat("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", &s) == 0) {
            f=fopen("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", "r");
            while(fgets(ligne, 255, f) != NULL) {
                if(strstr(ligne,"webftp_server") != NULL) {
                    strtok(ligne, "\r\n");
                    sysLv2FsUnlink(ligne);
                    CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_noncobra.sprx",ligne);
                    goto exit;
                }
            }
            fclose(f);
            f=fopen("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", "a");
            fputs("\r\n/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx", f);
            fclose(f);
            sysLv2FsUnlink("/dev_hdd0/webftp_server.sprx");
            CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_noncobra.sprx","/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx");
        }
    }

exit:

	if(add_mygame() != -2)  {
		sysLv2FsUnlink("/dev_hdd0/tmp/turnoff");
		{lv2syscall4(379,0x200,0,0,0);}
	}

    return 0;
}
