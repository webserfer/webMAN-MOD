#include <fcntl.h>
#include <ppu-lv2.h>
#include <sys/file.h>
#include <stdio.h>
#include <string.h>

#define SUCCESS 0
#define FAILED -1

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
	if (mem == NULL) return FAILED;

	while(pos < lenght)
	{
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
	if(fd >=0) sysLv2FsClose(fd);
	if(fd2>=0) sysLv2FsClose(fd2);
	if(ret>0) ret = SUCCESS;

	return ret;
}

int sys_get_version(u32 *version)
{
	lv2syscall2(8, 0x7000, (u64)version);
	return_to_user_prog(int);
}

bool is_cobra(void)
{
	u32 version = 0x99999999;
	if (sys_get_version(&version) < 0) return false;
	if (version != 0x99999999)         return true;
	return false;
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
	if(f==NULL) return FAILED;
	fseek (f , 0 , SEEK_END);
	size = ftell (f);
	fseek(f, 0, SEEK_SET);

	cat = (char*) malloc (sizeof(char)*size);
	if (cat == NULL) {free(cat); return FAILED;}

	result = fread(cat,1,size, f);
	if (result != size) {free (cat); fclose (f); return FAILED;}
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
	if(f==NULL) {free(cat); return FAILED;}
	fwrite(cat, 1, pos, f);
	fwrite(fb, 1, sizeof(fb), f);
	fwrite(&cat[pos], 1, size-pos, f);
	fclose(f);


	// set target path for category_game.xml
	strcpy(cat_path, "/dev_blind/vsh/resource/explore/xmb/category_game.xml");
	if(sysLv2FsStat(cat_path, &stat) != SUCCESS) {
		strcpy(cat_path, "/dev_habib/vsh/resource/explore/xmb/category_game.xml");
		if(sysLv2FsStat(cat_path, &stat) != SUCCESS) {
			strcpy(cat_path, "/dev_rewrite/vsh/resource/explore/xmb/category_game.xml");

			// mount /dev_blind if category_game.xml is not found
			if(sysLv2FsStat(cat_path, &stat) != SUCCESS) {
				if(sys_fs_mount("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_blind", 0) == SUCCESS) {
					strcpy(cat_path, "/dev_blind/vsh/resource/explore/xmb/category_game.xml");
					if(sysLv2FsStat(cat_path, &stat) != SUCCESS) { free(cat); return FAILED;}
				} else { free(cat); return FAILED;}
			}
		}
	}

	// rename category_game.xml as category_game.xml.bak
	char cat_path_bak[255];
	strcpy(cat_path_bak, cat_path);
	strcat(cat_path_bak, ".bak");
	sysLv2FsUnlink(cat_path_bak);
	if(sysLv2FsRename(cat_path, cat_path_bak) != SUCCESS) {free(cat); return FAILED;} ;

	// update category_game.xml
	if(CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/category_game.xml", cat_path) != SUCCESS) {
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

	sysLv2FsMkdir("/dev_hdd0/tmp", 0777);
	sysLv2FsMkdir("/dev_hdd0/tmp/wm_lang", 0777);

	// remove language files (old location)
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_EN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_AR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_CN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_DE.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_ES.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_FR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_GR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_DK.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_HU.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_HR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_BG.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_CZ.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_SK.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_IN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_JP.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_KR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_IT.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_NL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_PL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_PT.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_RU.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_TR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_ZH.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/LANG_XX.TXT");

	// remove language files
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_EN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_AR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_CN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_DE.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_ES.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_FR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_GR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_DK.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_HU.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_HR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_BG.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_CZ.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_SK.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_IN.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_JP.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_KR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_IT.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_NL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_PL.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_PT.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_RU.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_TR.TXT");
	sysLv2FsUnlink("/dev_hdd0/tmp/wm_lang/LANG_ZH.TXT");

	// remove old files
	sysLv2FsUnlink("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_rebug_cobra_multi19.sprx");
	sysLv2FsUnlink("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_rebug_cobra_multi20.sprx");
	sysLv2FsUnlink("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_rebug_cobra_multi21.sprx");

	// update languages
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_EN.TXT","/dev_hdd0/tmp/wm_lang/LANG_EN.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_AR.TXT","/dev_hdd0/tmp/wm_lang/LANG_AR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_CN.TXT","/dev_hdd0/tmp/wm_lang/LANG_CN.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_DE.TXT","/dev_hdd0/tmp/wm_lang/LANG_DE.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_ES.TXT","/dev_hdd0/tmp/wm_lang/LANG_ES.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_FR.TXT","/dev_hdd0/tmp/wm_lang/LANG_FR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_GR.TXT","/dev_hdd0/tmp/wm_lang/LANG_GR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_DK.TXT","/dev_hdd0/tmp/wm_lang/LANG_DK.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_HU.TXT","/dev_hdd0/tmp/wm_lang/LANG_HU.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_HR.TXT","/dev_hdd0/tmp/wm_lang/LANG_HR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_BG.TXT","/dev_hdd0/tmp/wm_lang/LANG_BG.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_CZ.TXT","/dev_hdd0/tmp/wm_lang/LANG_CZ.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_SK.TXT","/dev_hdd0/tmp/wm_lang/LANG_SK.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_IN.TXT","/dev_hdd0/tmp/wm_lang/LANG_IN.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_IT.TXT","/dev_hdd0/tmp/wm_lang/LANG_IT.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_JP.TXT","/dev_hdd0/tmp/wm_lang/LANG_JP.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_KR.TXT","/dev_hdd0/tmp/wm_lang/LANG_KR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_NL.TXT","/dev_hdd0/tmp/wm_lang/LANG_NL.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_PL.TXT","/dev_hdd0/tmp/wm_lang/LANG_PL.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_PT.TXT","/dev_hdd0/tmp/wm_lang/LANG_PT.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_RU.TXT","/dev_hdd0/tmp/wm_lang/LANG_RU.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_TR.TXT","/dev_hdd0/tmp/wm_lang/LANG_TR.TXT");
	CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_ZH.TXT","/dev_hdd0/tmp/wm_lang/LANG_ZH.TXT");


	sysLv2FsMkdir("/dev_hdd0/tmp/wm_icons", 0777);

	// copy new icons
	if((sysLv2FsStat("/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx", &s) == SUCCESS))
	{
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_album_ps3.png", "/dev_hdd0/tmp/wm_icons/icon_wm_album_ps3.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_album_psx.png", "/dev_hdd0/tmp/wm_icons/icon_wm_album_psx.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_album_ps2.png", "/dev_hdd0/tmp/wm_icons/icon_wm_album_ps2.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_album_psp.png", "/dev_hdd0/tmp/wm_icons/icon_wm_album_psp.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_album_dvd.png", "/dev_hdd0/tmp/wm_icons/icon_wm_album_dvd.png");

		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_ps3.png"      , "/dev_hdd0/tmp/wm_icons/icon_wm_ps3.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_psx.png"      , "/dev_hdd0/tmp/wm_icons/icon_wm_psx.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_ps2.png"      , "/dev_hdd0/tmp/wm_icons/icon_wm_ps2.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_psp.png"      , "/dev_hdd0/tmp/wm_icons/icon_wm_psp.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_dvd.png"      , "/dev_hdd0/tmp/wm_icons/icon_wm_dvd.png");

		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_settings.png" , "/dev_hdd0/tmp/wm_icons/icon_wm_settings.png");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/icon_wm_eject.png"    , "/dev_hdd0/tmp/wm_icons/icon_wm_eject.png"   );
	}

	// skip update custom language
	if(sysLv2FsStat("/dev_hdd0/tmp/wm_lang/LANG_XX.TXT", &s))
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/LANG_XX.TXT","/dev_hdd0/tmp/wm_lang/LANG_XX.TXT");

	// update PRX+Mamba Loader
	if((sysLv2FsStat("/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx", &s) == SUCCESS))
	{
		sysLv2FsChmod("/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx", 0777);
		sysLv2FsUnlink("/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx");

		if((sysLv2FsStat("/dev_flash/rebug", &s) == SUCCESS))
			CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_rebug_cobra_multi23.sprx", "/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx");
		else
			CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server.sprx", "/dev_hdd0/game/IRISMAN01/USRDIR/webftp_server.sprx");
	}

	char ligne[255];
	FILE* f=NULL;

	// update PRX Loader
	if(sysLv2FsStat("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", &s) == SUCCESS)
	{
		f=fopen("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", "r");
		while(fgets(ligne, 255, f) != NULL) {
			if(strstr(ligne,"webftp_server") != NULL) {
				fclose(f);
				strtok(ligne, "\r\n");
				sysLv2FsUnlink(ligne);
				CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_noncobra.sprx",ligne);
				goto cont;
			}
		}
		fclose(f);
		f=fopen("/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt", "a");
		fputs("\r\n/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx", f);
		fclose(f);

		sysLv2FsChmod("/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx", 0777);
		sysLv2FsUnlink("/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx");

		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_noncobra.sprx","/dev_hdd0/game/PRXLOADER/USRDIR/webftp_server_noncobra.sprx");
	}

cont:

	// update dev_flash (rebug)
	if((sysLv2FsStat("/dev_flash/vsh/module/webftp_server.sprx", &s) == SUCCESS))
	{
		if(sysLv2FsStat("/dev_blind/vsh/module/webftp_server.sprx", &s) != SUCCESS)
			sys_fs_mount("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_blind", 0);

		sysLv2FsChmod("/dev_blind/vsh/module/webftp_server.sprx", 0777);
		sysLv2FsUnlink("/dev_blind/vsh/module/webftp_server.sprx");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server_rebug_cobra_multi23.sprx", "/dev_blind/vsh/module/webftp_server.sprx");

		// delete webMAN from hdd0
		if((sysLv2FsStat("/dev_blind/vsh/module/webftp_server.sprx", &s) == SUCCESS)) {
			sysLv2FsChmod("/dev_hdd0/webftp_server.sprx", 0777);
			sysLv2FsUnlink("/dev_hdd0/webftp_server.sprx");

			sysLv2FsChmod("/dev_hdd0/plugins/webftp_server.sprx", 0777);
			sysLv2FsUnlink("/dev_hdd0/plugins/webftp_server.sprx");

			if(sysLv2FsStat("/dev_hdd0/boot_plugins.txt", &s) == SUCCESS) {
				f=fopen("/dev_hdd0/boot_plugins.txt", "r");
				while(fgets(ligne, 255, f) != NULL) {
					if(strstr(ligne,"webftp_server") != NULL && strstr(ligne,"/dev_blind") == NULL) {
						strtok(ligne, "\r\n");
						sysLv2FsChmod(ligne, 0777);
						sysLv2FsUnlink(ligne);
						break;
					}
				}
				fclose(f);
			}
		}

		// reboot
		sysLv2FsUnlink("/dev_hdd0/tmp/turnoff");
		{lv2syscall4(379,0x200,0,0,0);}
		return 0;
	}

	// update boot_plugins.txt
	if(is_cobra()) {
		if(sysLv2FsStat("/dev_hdd0/boot_plugins.txt", &s) == SUCCESS) {
			f=fopen("/dev_hdd0/boot_plugins.txt", "r");
			while(fgets(ligne, 255, f) != NULL) {
				if(strstr(ligne,"webftp_server.sprx") != NULL) {
					fclose(f);
					strtok(ligne, "\r\n");
					sysLv2FsChmod(ligne, 0777);
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

		sysLv2FsChmod("/dev_hdd0/webftp_server.sprx", 0777);
		sysLv2FsUnlink("/dev_hdd0/webftp_server.sprx");
		CopyFile("/dev_hdd0/game/UPDWEBMOD/USRDIR/webftp_server.sprx", "/dev_hdd0/webftp_server.sprx");
	}

exit:

	// update category_game.xml (add fb.xml)
	if(add_mygame() != -2)
	{
		// reboot
		sysLv2FsUnlink("/dev_hdd0/tmp/turnoff");
		{lv2syscall4(379,0x200,0,0,0);}
	}

	return 0;
}
