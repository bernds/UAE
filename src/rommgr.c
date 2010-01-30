 /*
  * UAE - The Un*x Amiga Emulator
  *
  * ROM file management
  *
  * (c) 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "uae.h"
#include "memory.h"
#include "romlist.h"
#include "zfile.h"
#include "gui.h"
#include "crc32.h"

static int is_encrypted_rom (uae_u8 *data, int size)
{
    return size > 11 && !memcmp (data, "AMIROMTYPE1", 11);
}

static struct romlist *list_of_roms;
static int romlist_cnt;

void romlist_add (char *path, struct romdata *rd, int loc)
{
    struct romlist *rl2;

    romlist_cnt++;
    list_of_roms = realloc (list_of_roms, sizeof (struct romlist) * romlist_cnt);
    rl2 = list_of_roms + romlist_cnt - 1;
    rl2->path = my_strdup (path);
    rl2->rd = rd;
    rl2->loc = loc;
}

char *romlist_get (struct romdata *rd)
{
    int i;

    if (!rd)
	return 0;
    for (i = 0; i < romlist_cnt; i++) {
	if (list_of_roms[i].rd == rd)
	    return list_of_roms[i].path;
    }
    return 0;
}

struct romlist *romlist_from_idx (int idx, int type, int need_crc32)
{
    int i;
    for (i = 0; i < romlist_cnt; i++) {
	if (!list_of_roms[i].rd)
	    continue;

	if (list_of_roms[i].rd->type != type || (!list_of_roms[i].rd->crc32 && need_crc32))
	    continue;
	
	if (idx == 0)
	    return list_of_roms + i;
	idx--;
    }
    return 0;
}

static void sort_romlist (void)
{
    int i, j;
    for (i = 0; i < romlist_cnt; i++) {
	int k = i;

	if (list_of_roms[i].rd == 0)
	    continue;

	for (j = i + 1; j < romlist_cnt; j++) {
	    if (list_of_roms[j].rd == 0)
		continue;
	    if (list_of_roms[j].rd == list_of_roms[k].rd
		&& list_of_roms[j].loc == ROMLOC_USER)
		k = j;
	    else if (strcmp (list_of_roms[j].rd->name, list_of_roms[k].rd->name) < 0)
		k = j;
	}
	if (k != i) {
	    struct romlist t;
	    t = list_of_roms[k];
	    list_of_roms[k] = list_of_roms[i];
	    list_of_roms[i] = t;
	}
    }
    /* Delete duplicates from the user directory.  */
    for (i = j = 0; i < romlist_cnt; i++) {
	if (!list_of_roms[i].rd
	    || i + 1 > romlist_cnt
	    || list_of_roms[i].rd != list_of_roms[i + 1].rd
	    || list_of_roms[i].loc == ROMLOC_SYSTEM)
	{
	    if (i != j)
		list_of_roms[j] = list_of_roms[i];
	    j++;
	}
    }
    romlist_cnt = j;
}

void romlist_clear (int mask)
{
    int i, j;
    for (i = j = 0; i < romlist_cnt; i++) {
	if ((list_of_roms[i].loc & mask) == 0) {
	    if (i != j)
		list_of_roms[j] = list_of_roms[i];
	    j++;
	}
    }
    romlist_cnt = j;
    list_of_roms = realloc (list_of_roms, sizeof (struct romlist) * j);
}

struct romdata *getromdatabypath(char *path)
{
    int i;
    for (i = 0; i < romlist_cnt; i++) {
	struct romdata *rd = list_of_roms[i].rd;
	if (rd->configname && path[0] == ':') {
	    if (!strcmp(path + 1, rd->configname))
		return rd;
	}
	if (!strcmp(list_of_roms[i].path, path))
	    return list_of_roms[i].rd;
    }
    return NULL;
}

#define NEXT_ROM_ID 68

static struct romheader romheaders[] = {
    { "Freezer Cartridges", 1 },
    { "Arcadia Games", 2 },
    { NULL, 0 }
};

static struct romdata roms[] = {
    { "Cloanto Amiga Forever ROM key", 0, 0, 0, 0, 0, 2069, 0, 0, 1, ROMTYPE_KEY, 0,
      0x869ae1b1, 0x801bbab3,0x2e3d3738,0x6dd1636d,0x4f1d6fa7,0xe21d5874, NULL },
    { "Cloanto Amiga Forever 2006 ROM key", 0, 0, 0, 0, 0, 750, 48, 0, 1, ROMTYPE_KEY, 0,
	0xb01c4b56, 0xbba8e5cd,0x118b8d92,0xafed5693,0x5eeb9770,0x2a662d8f, NULL },

    { "KS ROM v1.0 (A1000)(NTSC)", 1, 0, 1, 0, "A1000\0", 262144, 1, 0, 0, ROMTYPE_KICK, 0,
	0x299790ff, 0x00C15406,0xBEB4B8AB,0x1A16AA66,0xC05860E1,0xA7C1AD79, NULL },
    { "KS ROM v1.1 (A1000)(NTSC)", 1, 1, 31, 34, "A1000\0", 262144, 2, 0, 0, ROMTYPE_KICK, 0,
	0xd060572a, 0x4192C505,0xD130F446,0xB2ADA6BD,0xC91DAE73,0x0ACAFB4C, NULL },
    { "KS ROM v1.1 (A1000)(PAL)", 1, 1, 31, 34, "A1000\0", 262144, 3, 0, 0, ROMTYPE_KICK, 0,
	0xec86dae2, 0x16DF8B5F,0xD524C5A1,0xC7584B24,0x57AC15AF,0xF9E3AD6D, NULL },
    { "KS ROM v1.2 (A1000)", 1, 2, 33, 166, "A1000\0", 262144, 4, 0, 0, ROMTYPE_KICK, 0,
	0x9ed783d0, 0x6A7BFB5D,0xBD6B8F17,0x9F03DA84,0xD8D95282,0x67B6273B, NULL },
    { "KS ROM v1.2 (A500,A1000,A2000)", 1, 2, 33, 180, "A500\0A1000\0A2000\0", 262144, 5, 0, 0, ROMTYPE_KICK, 0,
	0xa6ce1636, 0x11F9E62C,0xF299F721,0x84835B7B,0x2A70A163,0x33FC0D88, NULL },
    { "KS ROM v1.3 (A500,A1000,A2000)", 1, 3, 34, 5, "A500\0A1000\0A2000\0", 262144, 6, 0, 0, ROMTYPE_KICK, 0,
	0xc4f0f55f, 0x891E9A54,0x7772FE0C,0x6C19B610,0xBAF8BC4E,0xA7FCB785, NULL },
    { "KS ROM v1.3 (A3000)(SK)", 1, 3, 34, 5, "A3000\0", 262144, 32, 0, 0, ROMTYPE_KICK, 0,
	0xe0f37258, 0xC39BD909,0x4D4E5F4E,0x28C1411F,0x30869504,0x06062E87, NULL },
    { "KS ROM v1.4 (A3000)", 1, 4, 36, 16, "A3000\0", 524288, 59, ROMREQ_68030, 0, ROMTYPE_KICK, 0,
	0xbc0ec13f, 0xF76316BF,0x36DFF14B,0x20FA349E,0xD02E4B11,0xDD932B07, NULL },

    { "KS ROM v2.04 (A500+)", 2, 4, 37, 175, "A500+\0", 524288, 7, 0, 0, ROMTYPE_KICK, 0,
	0xc3bdb240, 0xC5839F5C,0xB98A7A89,0x47065C3E,0xD2F14F5F,0x42E334A1, NULL },
    { "KS ROM v2.05 (A600)", 2, 5, 37, 299, "A600\0", 524288, 8, 0, 0, ROMTYPE_KICK, 0,
	0x83028fb5, 0x87508DE8,0x34DC7EB4,0x7359CEDE,0x72D2E3C8,0xA2E5D8DB, NULL },
    { "KS ROM v2.05 (A600HD)", 2, 5, 37, 300, "A600HD\0A600\0", 524288, 9, 0, 0, ROMTYPE_KICK, 0,
	0x64466c2a, 0xF72D8914,0x8DAC39C6,0x96E30B10,0x859EBC85,0x9226637B, NULL },
    { "KS ROM v2.05 (A600HD)", 2, 5, 37, 350, "A600HD\0A600\0", 524288, 10, 0, 0, ROMTYPE_KICK, 0,
	0x43b0df7b, 0x02843C42,0x53BBD29A,0xBA535B0A,0xA3BD9A85,0x034ECDE4, NULL },

    { "KS ROM v3.0 (A1200)", 3, 0, 39, 106, "A1200\0", 524288, 11, 0, 0, ROMTYPE_KICK, 0,
	0x6c9b07d2, 0x70033828,0x182FFFC7,0xED106E53,0x73A8B89D,0xDA76FAA5, NULL },
    { "KS ROM v3.0 (A4000)", 3, 0, 39, 106, "A4000\0", 524288, 12, ROMREQ_A4000, 0, ROMTYPE_KICK, 0,
	0x9e6ac152, 0xF0B4E9E2,0x9E12218C,0x2D5BD702,0x0E4E7852,0x97D91FD7, NULL },
    { "KS ROM v3.1 (A4000)", 3, 1, 40, 70, "A4000\0", 524288, 13, ROMREQ_A4000, 0, ROMTYPE_KICK, 0,
	0x2b4566f1, 0x81c631dd,0x096bbb31,0xd2af9029,0x9c76b774,0xdb74076c, NULL },
    { "KS ROM v3.1 (A500,A600,A2000)", 3, 1, 40, 63, "A500\0A600\0A2000\0", 524288, 14, 0, 0, ROMTYPE_KICK, 0,
	0xfc24ae0d, 0x3B7F1493,0xB27E2128,0x30F989F2,0x6CA76C02,0x049F09CA, NULL },
    { "KS ROM v3.1 (A1200)", 3, 1, 40, 68, "A1200\0", 524288, 15, ROMREQ_68EC020, 0, ROMTYPE_KICK, 0,
	0x1483a091, 0xE2154572,0x3FE8374E,0x91342617,0x604F1B3D,0x703094F1, NULL },
    { "KS ROM v3.1 (A3000)", 3, 1, 40, 68, "A3000\0", 524288, 61, ROMREQ_68020, 0, ROMTYPE_KICK, 0,
	0xefb239cc, 0xF8E210D7,0x2B4C4853,0xE0C9B85D,0x223BA20E,0x3D1B36EE, NULL },
    { "KS ROM v3.1 (A4000)(Cloanto)", 3, 1, 40, 68, "A4000\0", 524288, 31, ROMREQ_A4000, 1, ROMTYPE_KICK, 0,
	0x43b6dd22, 0xC3C48116,0x0866E60D,0x085E436A,0x24DB3617,0xFF60B5F9, NULL },
    { "KS ROM v3.1 (A4000)", 3, 1, 40, 68, "A4000\0", 524288, 16, ROMREQ_A4000, 0, ROMTYPE_KICK, 0,
	0xd6bae334, 0x5FE04842,0xD04A4897,0x20F0F4BB,0x0E469481,0x99406F49, NULL },
    { "KS ROM v3.1 (A4000T)", 3, 1, 40, 70, "A4000T\0", 524288, 17, ROMREQ_A4000, 0, ROMTYPE_KICK, 0,
	0x75932c3a, 0xB0EC8B84,0xD6768321,0xE01209F1,0x1E6248F2,0xF5281A21, NULL },
    { "KS ROM v3.X (A4000)(Cloanto)", 3, 10, 45, 57, "A4000\0", 524288, 46, ROMREQ_A4000, 0, ROMTYPE_KICK, 0,
	0x08b69382, 0x81D3AEA3,0x0DB7FBBB,0x4AFEE41C,0x21C5ED66,0x2B70CA53, NULL },

    { "CD32 KS ROM v3.1", 3, 1, 40, 60, "CD32\0", 524288, 18, ROMREQ_68EC020, 0, ROMTYPE_KICKCD32, 0,
	0x1e62d4a5, 0x3525BE88,0x87F79B59,0x29E017B4,0x2380A79E,0xDFEE542D, NULL },
    { "CD32 extended ROM", 3, 1, 40, 60, "CD32\0", 524288, 19, ROMREQ_68EC020, 0, ROMTYPE_EXTCD32, 0,
	0x87746be2, 0x5BEF3D62,0x8CE59CC0,0x2A66E6E4,0xAE0DA48F,0x60E78F7F, NULL },
    { "CD32 ROM (KS + extended)", 3, 1, 40, 60, "CD32\0", 2 * 524288, 64, ROMREQ_68EC020, 0, ROMTYPE_KICKCD32, 0,
	0xd3837ae4, 0x06807db3,0x18163745,0x5f4d4658,0x2d9972af,0xec8956d9, NULL },

    { "CDTV extended ROM v1.00", 1, 0, 1, 0, "CDTV\0", 262144, 20, 0, 0, ROMTYPE_EXTCDTV, 0,
	0x42baa124, 0x7BA40FFA,0x17E500ED,0x9FED041F,0x3424BD81,0xD9C907BE, NULL },
    { "CDTV extended ROM v2.07", 2, 7, 2, 7, "CDTV\0", 262144, 22, 0, 0, ROMTYPE_EXTCDTV, 0,
	0xceae68d2, 0x5BC114BB,0xA29F60A6,0x14A31174,0x5B3E2464,0xBFA06846, NULL },
    { "CDTV extended ROM v2.30", 2, 30, 2, 30, "CDTV\0", 262144, 21, 0, 0, ROMTYPE_EXTCDTV, 0,
	0x30b54232, 0xED7E461D,0x1FFF3CDA,0x321631AE,0x42B80E3C,0xD4FA5EBB, NULL },

    { "A1000 bootstrap ROM", 0, 0, 0, 0, "A1000\0", 8192, 23, ROMREQ_A1000, 0, ROMTYPE_KICK, 0,
	0x62f11c04, 0xC87F9FAD,0xA4EE4E69,0xF3CCA0C3,0x6193BE82,0x2B9F5FE6, NULL },
    { "A1000 bootstrap ROM", 0, 0, 0, 0, "A1000\0", 65536, 24, ROMREQ_A1000, 0, ROMTYPE_KICK, 0,
	0x0b1ad2d0, 0xBA93B8B8,0x5CA0D83A,0x68225CC3,0x3B95050D,0x72D2FDD7, NULL },

    { "Freezer: Action Replay Mk I v1.00", 1, 0, 1, 0, "AR\0", 65536, 52, 0, 0, ROMTYPE_AR, 1,
	0x2d921771, 0x1EAD9DDA,0x2DAD2914,0x6441F5EF,0x72183750,0x22E01248, NULL },
    { "Freezer: Action Replay Mk I v1.50", 1, 50, 1, 50, "AR\0", 65536, 25, 0, 0, ROMTYPE_AR, 1,
	0xd4ce0675, 0x843B433B,0x2C56640E,0x045D5FDC,0x854DC6B1,0xA4964E7C, NULL },
    { "Freezer: Action Replay Mk II v2.05", 2, 5, 2, 5, "AR\0", 131072, 26, 0, 0, ROMTYPE_AR, 1,
	0x1287301f, 0xF6601DE8,0x888F0050,0x72BF562B,0x9F533BBC,0xAF1B0074, NULL },
    { "Freezer: Action Replay Mk II v2.12", 2, 12, 2, 12, "AR\0", 131072, 27, 0, 0, ROMTYPE_AR, 1,
	0x804d0361, 0x3194A07A,0x0A82D8B5,0xF2B6AEFA,0x3CA581D6,0x8BA8762B, NULL },
    { "Freezer: Action Replay Mk II v2.14", 2, 14, 2, 14, "AR\0", 131072, 28, 0, 0, ROMTYPE_AR, 1,
	0x49650e4f, 0x255D6DF6,0x3A4EAB0A,0x838EB1A1,0x6A267B09,0x59DFF634, NULL },
    { "Freezer: Action Replay Mk III v3.09", 3, 9, 3, 9, "AR\0", 262144, 29, 0, 0, ROMTYPE_AR, 1,
	0x0ed9b5aa, 0x0FF3170A,0xBBF0CA64,0xC9DD93D6,0xEC0C7A01,0xB5436824, NULL },
    { "Freezer: Action Replay Mk III v3.17", 3, 17, 3, 17, "AR\0", 262144, 30, 0, 0, ROMTYPE_AR, 1,
	0xc8a16406, 0x5D4987C2,0xE3FFEA8B,0x1B02E314,0x30EF190F,0x2DB76542, NULL },
    { "Freezer: Action Replay 1200", 0, 0, 0, 0, "AR\0", 262144, 47, 0, 0, ROMTYPE_AR, 1,
	0x8d760101, 0x0F6AB834,0x2810094A,0xC0642F62,0xBA42F78B,0xC0B07E6A, NULL },

    { "Freezer: Action Cartridge Super IV Professional", 0, 0, 0, 0, "SUPERIV\0", 0, 62, 0, 0, ROMTYPE_SUPERIV, 1,
	0xffffffff, 0, 0, 0, 0, 0, "SuperIV" },
    { "Freezer: Action Cart. Super IV Pro (+ROM v4.3)", 4, 3, 4, 3, "SUPERIV\0", 170368, 60, 0, 0, ROMTYPE_SUPERIV, 1,
	0xe668a0be, 0x633A6E65,0xA93580B8,0xDDB0BE9C,0x9A64D4A1,0x7D4B4801, NULL },
    { "Freezer: X-Power Professional 500 v1.2", 1, 2, 1, 2, "XPOWER\0", 131072, 65, 0, 0, ROMTYPE_SUPERIV, 1,
	0x9e70c231, 0xa2977a1c,0x41a8ca7d,0x4af4a168,0x726da542,0x179d5963, NULL },
    /* v1.0 is bad dump */
    { "Freezer: Nordic Power v1.0", 0, 0, 0, 0, "NPOWER\0", 65536, 66, 0, 0, ROMTYPE_SUPERIV, 1, },
	//0xdd16cdec, 0xfd882967,0x87e2da5f,0x4ef6be32,0x5f7c9324,0xb5bd8e64 },
    { "Freezer: Nordic Power v2.0", 2, 0, 2, 0, "NPOWER\0", 65536, 67, 0, 0, ROMTYPE_SUPERIV, 1,
	0xa4db2906, 0x0aec68f7,0x25470c89,0x6b699ff4,0x6623dec5,0xc777466e },

    { "Freezer: HRTMon v2.30 (built-in)", 0, 0, 0, 0, "HRTMON\0", 0, 63, 0, 0, ROMTYPE_HRTMON, 1,
	0xffffffff, 0, 0, 0, 0, 0, "HRTMon" },

    { "A590/A2091 SCSI boot ROM", 0, 0, 6, 0, "A590\0A2091\0", 16384, 53, 0, 0, ROMTYPE_A2091BOOT, 0,
	0x8396cf4e, 0x5E03BC61,0x8C862ABE,0x7BF79723,0xB4EEF4D2,0x1859A0F2, NULL },
    { "A590/A2091 SCSI boot ROM", 0, 0, 6, 6, "A590\0A2091\0", 16384, 54, 0, 0, ROMTYPE_A2091BOOT, 0,
	0x33e00a7a, 0x739BB828,0xE874F064,0x9360F59D,0x26B5ED3F,0xBC99BB66, NULL },
    { "A590/A2091 SCSI boot ROM", 0, 0, 7, 0, "A590\0A2091\0", 16384, 55, 0, 0, ROMTYPE_A2091BOOT, 0,
	0x714a97a2, 0xE50F01BA,0xF2899892,0x85547863,0x72A82C33,0x3C91276E, NULL },
    { "A590/A2091 SCSI Guru boot ROM", 0, 0, 6, 14, "A590\0A2091\0", 32768, 56, 0, 0, ROMTYPE_A2091BOOT, 0,
	0x04e52f93, 0x6DA21B6F,0x5E8F8837,0xD64507CD,0x8A4D5CDC,0xAC4F426B, NULL },
    { "A4091 SCSI boot ROM", 0, 0, 40, 9, "A4091\0", 32768, 57, 0, 0, ROMTYPE_A4091BOOT, 0,
	0x00000000, 0, 0, 0, 0, 0, NULL },
    { "A4091 SCSI boot ROM", 0, 0, 40, 13, "A4091\0", 32768, 58, 0, 0, ROMTYPE_A4091BOOT, 0,
	0x54cb9e85, 0x3CE66919,0xF6FD6797,0x4923A12D,0x91B730F1,0xFFB4A7BA, NULL },

    { "Arcadia OnePlay 2.11", 0, 0, 0, 0, "ARCADIA\0", 0, 49, 0, 0, ROMTYPE_ARCADIABIOS, 0 },
    { "Arcadia TenPlay 2.11", 0, 0, 0, 0, "ARCADIA\0", 0, 50, 0, 0, ROMTYPE_ARCADIABIOS, 0 },
    { "Arcadia OnePlay 3.00", 0, 0, 0, 0, "ARCADIA\0", 0, 51, 0, 0, ROMTYPE_ARCADIABIOS, 0 },

    { "Arcadia SportTime Table Hockey", 0, 0, 0, 0, "ARCADIA\0", 0, 33, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia SportTime Bowling", 0, 0, 0, 0, "ARCADIA\0", 0, 34, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia World Darts", 0, 0, 0, 0, "ARCADIA\0", 0, 35, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Magic Johnson's Fast Break", 0, 0, 0, 0, "ARCADIA\0", 0, 36, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Leader Board Golf", 0, 0, 0, 0, "ARCADIA\0", 0, 37, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Leader Board Golf (alt)", 0, 0, 0, 0, "ARCADIA\0", 0, 38, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Ninja Mission", 0, 0, 0, 0, "ARCADIA\0", 0, 39, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Road Wars", 0, 0, 0, 0, "ARCADIA\0", 0, 40, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Sidewinder", 0, 0, 0, 0, "ARCADIA\0", 0, 41, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Spot", 0, 0, 0, 0, "ARCADIA\0", 0, 42, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Space Ranger", 0, 0, 0, 0, "ARCADIA\0", 0, 43, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia Xenon", 0, 0, 0, 0, "ARCADIA\0", 0, 44, 0, 0, ROMTYPE_ARCADIAGAME, 2 },
    { "Arcadia World Trophy Soccer", 0, 0, 0, 0, "ARCADIA\0", 0, 45, 0, 0, ROMTYPE_ARCADIAGAME, 2 },

    { NULL }

};

struct romdata *getarcadiarombyname (char *name)
{
    int i;
    for (i = 0; roms[i].name; i++) {
	if (roms[i].type == ROMTYPE_ARCADIAGAME || roms[i].type == ROMTYPE_ARCADIAGAME) {
	    char *p = roms[i].name;
	    p = p + strlen (p) + 1;
	    if (strlen (name) >= strlen (p) + 4) {
		char *p2 = name + strlen (name) - strlen (p) - 4;
		if (!memcmp (p, p2, strlen (p)) && !memcmp (p2 + strlen (p2) - 4, ".zip", 4))
		    return &roms[i];
	    }
	}
    }
    return NULL;
}

struct romlist **getarcadiaroms(void)
{
    int i, out, max;
    void *buf;
    struct romlist **rdout, *rltmp;

    max = 0;
    for (i = 0; roms[i].name; i++) {
	if (roms[i].type == ROMTYPE_ARCADIABIOS || roms[i].type == ROMTYPE_ARCADIAGAME)
	    max++;
    }
    buf = xmalloc((sizeof (struct romlist*) + sizeof (struct romlist)) * (max + 1));
    rdout = (struct romlist**)buf;
    rltmp = (struct romlist*)((uae_u8*)buf + (max + 1) * sizeof (struct romlist*));
    out = 0;
    for (i = 0; roms[i].name; i++) {
	if (roms[i].type == ROMTYPE_ARCADIABIOS || roms[i].type == ROMTYPE_ARCADIAGAME) {
	    rdout[out++] = rltmp;
	    rltmp->path = NULL;
	    rltmp->rd = &roms[i];
	    rltmp++;
	}
    }
    rdout[out] = NULL;
    return rdout;
}

static int kickstart_checksum_do (uae_u8 *mem, int size)
{
    uae_u32 cksum = 0, prevck = 0;
    int i;
    for (i = 0; i < size; i+=4) {
	uae_u32 data = mem[i]*65536*256 + mem[i+1]*65536 + mem[i+2]*256 + mem[i+3];
	cksum += data;
	if (cksum < prevck)
	    cksum++;
	prevck = cksum;
    }
    return cksum == 0xffffffff;
}

#define ROM_KEY_NUM 3
struct rom_key {
    uae_u8 *key;
    int size;
};

static struct rom_key keyring[ROM_KEY_NUM];
int nr_keys;

static int addkey (uae_u8 *key, int size, const char *name)
{
    int i;

    if (nr_keys == ROM_KEY_NUM)
	return 0;

    for (i = 0; i < nr_keys; i++) {
	if (keyring[i].size == size && !memcmp (keyring[i].key, key, size))
	    return 0;
    }
    keyring[nr_keys].key = malloc (size);
    if (!keyring[nr_keys].key)
	return 0;
    memcpy (keyring[nr_keys].key, key, size);
    keyring[nr_keys].size = size;
    write_log ("ROM KEY '%s' %d bytes loaded\n", name, size);
    nr_keys++;
    return 1;
}

static void decode_cloanto_rom_do (uae_u8 *mem, int size, int real_size, uae_u8 *key, int keysize)
{
    int cnt, t;
    for (t = cnt = 0; cnt < size; cnt++, t = (t + 1) % keysize)  {
	mem[cnt] ^= key[t];
	if (real_size == cnt + 1)
	    t = keysize - 1;
    }
}

int decode_cloanto_rom (uae_u8 *mem, int size, int real_size, int quiet)
{
    struct zfile *keyf;
    uae_u8 *p;
    int keysize;
    int i;

    /* Try keyring entries first.  */
    for (i = ROM_KEY_NUM - 1; i >= 0; i--) {
	uae_u8 *key = keyring[i].key;
	keysize = keyring[i].size;
	if (!key)
	    continue;
	decode_cloanto_rom_do (mem, size, real_size, key, keysize);

	if ((mem[2] == 0x4e && mem[3] == 0xf9)
	    || (mem[0] == 0x11 && (mem[1] == 0x11 || mem[1] == 0x14)))
	    return 1;
	/* Undo the decode.  */
	decode_cloanto_rom_do (mem, size, real_size, key, keysize);
    }

    if (strlen (currprefs.keyfile) == 0) {
	if (!quiet)
	    gui_message ("No filename given for ROM key file and ROM image is an encrypted \"Amiga Forever\" ROM file.\n");
	return 0;
    }
    keyf = zfile_open (currprefs.keyfile, "rb");
    if (keyf == 0) {
	if (!quiet)
	    gui_message ("Could not find specified ROM key-file.\n");
	return 0;
    }

    p = (uae_u8 *) xmalloc (524288);
    keysize = zfile_fread (p, 1, 524288, keyf);
    decode_cloanto_rom_do (mem, size, real_size, p, keysize);
    zfile_fclose (keyf);
    free (p);
    return 1;
}

#if 0
struct romdata *getromdatabyname (char *name)
{
    char tmp[MAX_PATH];
    int i = 0;
    while (roms[i].name) {
	getromname (&roms[i], tmp);
	if (!strcmp (tmp, name) || !strcmp (roms[i].name, name))
	    return &roms[i];
	i++;
    }
    return 0;
}
#endif

struct romdata *getromdatabyid (int id)
{
    int i = 0;
    while (roms[i].name) {
	if (id == roms[i].id)
	    return &roms[i];
	i++;
    }
    return 0;
}

STATIC_INLINE int notcrc32(uae_u32 crc32)
{
    if (crc32 == 0xffffffff || crc32 == 0x00000000)
	return 1;
    return 0;
}

struct romdata *getromdatabycrc (uae_u32 crc32)
{
    int i = 0;
    while (roms[i].name) {
	if (crc32 == roms[i].crc32 && !notcrc32(crc32))
	    return &roms[i];
	i++;
    }
    return 0;
}

static int cmpsha1(uae_u8 *s1, struct romdata *rd)
{
    int i;

    for (i = 0; i < SHA1_SIZE / 4; i++) {
	uae_u32 v1 = (s1[0] << 24) | (s1[1] << 16) | (s1[2] << 8) | (s1[3] << 0);
	uae_u32 v2 = rd->sha1[i];
	if (v1 != v2)
	    return -1;
	s1 += 4;
    }
    return 0;
}

static struct romdata *checkromdata (uae_u8 *sha1, int size, uae_u32 mask)
{
    int i = 0;
    while (roms[i].name) {
	if (!notcrc32 (roms[i].crc32) && roms[i].size >= size) {
	    if (roms[i].type & mask) {
		if (!cmpsha1 (sha1, &roms[i]))
		    return &roms[i];
	    }
	}
	i++;
    }
    return NULL;
}

struct romdata *getromdatabydata (uae_u8 *rom, int size)
{
    uae_u8 sha1[SHA1_SIZE];
    uae_u8 tmp[4];
    uae_u8 *tmpbuf = NULL;
    struct romdata *ret = NULL;

    if (is_encrypted_rom (rom, size)) {
	uae_u8 *tmpbuf = (uae_u8*)xmalloc (size);
	int tmpsize = size - 11;
	memcpy (tmpbuf, rom + 11, tmpsize);
	if (!decode_cloanto_rom (tmpbuf, tmpsize, tmpsize, 1))
	    return 0;
	rom = tmpbuf;
	size = tmpsize;
    }
    get_sha1 (rom, size, sha1);
    ret = checkromdata(sha1, size, -1);
    if (!ret) {
	get_sha1 (rom, size / 2, sha1);
	ret = checkromdata (sha1, size / 2, -1);
	if (!ret) {
	    /* ignore AR IO-port range until we have full dump */
	    memcpy (tmp, rom, 4);
	    memset (rom, 0, 4);
	    get_sha1 (rom, size, sha1);
	    ret = checkromdata (sha1, size, ROMTYPE_AR);
	    memcpy (rom, tmp, 4);
	}
    }
    free (tmpbuf);
    return ret;
}

static struct romdata *getromdatabyzfile (struct zfile *f)
{
    int pos, size;
    uae_u8 *p;
    struct romdata *rd;

    pos = zfile_ftell (f);
    zfile_fseek (f, 0, SEEK_END);
    size = zfile_ftell (f);

    /* Weed out too-large files to save time.  */
    if (size > 1024 * 1024)
	return 0;

    p = (uae_u8*)xmalloc (size);
    if (!p)
	return 0;
    memset (p, 0, size);
    zfile_fseek (f, 0, SEEK_SET);
    zfile_fread (p, 1, size, f);
    zfile_fseek (f, pos, SEEK_SET);
    rd = getromdatabydata (p, size);
    free (p);
    return rd;
}

void getromname	(struct romdata *rd, char *name)
{
    name[0] = 0;
    if (!rd)
	return;
    strcat (name, rd->name);
    if ((rd->subrev || rd->subver) && rd->subver != rd->ver)
	sprintf (name + strlen (name), " rev %d.%d", rd->subver, rd->subrev);
    if (rd->size > 0)
	sprintf (name + strlen (name), " (%dk)", (rd->size + 1023) / 1024);
}

struct romlist *getromlistbyids(int *ids)
{
    struct romdata *rd;
    int i, j;

    i = 0;
    while (ids[i] >= 0) {
	rd = getromdatabyid (ids[i]);
	if (rd) {
	    for (j = 0; j < romlist_cnt; j++) {
		if (list_of_roms[j].rd == rd)
		    return list_of_roms + j;
	    }
	}
	i++;
    }
    return NULL;
}

static struct romlist *getromlistbyromdata(struct romdata *rd)
{
    int ids[2];
    
    ids[0] = rd->id;
    ids[1] = 0;
    return getromlistbyids(ids);
}

#if 0
void romwarning(int *ids)
{
    int i, exp;
    char tmp1[MAX_DPATH], tmp2[MAX_DPATH];
    char tmp3[MAX_DPATH];

    exp = 0;
    tmp2[0] = 0;
    i = 0;
    while (ids[i] >= 0) {
	struct romdata *rd = getromdatabyid (ids[i]);
	getromname (rd, tmp1);
	strcat (tmp2, "- ");
	strcat (tmp2, tmp1);
	strcat (tmp2, "\n");
	if (rd->type & (ROMTYPE_A2091BOOT | ROMTYPE_A4091BOOT))
	    exp++;
	i++;
    }
    translate_message (exp ? NUMSG_EXPROMNEED : NUMSG_ROMNEED, tmp3);
    gui_message (tmp3, tmp2);
}
#endif

void scan_roms (const char *path, int loc)
{
    DIR *dir;
    int pathlen = strlen (path);
    int bufsz = pathlen + 256;
    char *buffer;
    uae_u8 *data;
    int keys_added = 0;

    dir = opendir (path);
    if (!dir)
	return;

    buffer = malloc (bufsz);
    if (!buffer)
	goto out;
    data = malloc (1024 * 1024);
    if (!data)
	goto out1;
    romlist_clear (loc);

    strcpy (buffer, path);
    buffer[pathlen++] = '/';
    buffer[pathlen] = '\0';
    for (;;) {
	struct zfile *f;
	struct dirent *ent = readdir (dir);
	int len;
	struct romdata *rd;
	long size;

	if (!ent)
	    break;

	len = strlen (ent->d_name);
	if (len + pathlen + 1 >= bufsz) {
	    bufsz = len + pathlen + 200;
	    buffer = realloc (buffer, bufsz);
	    if (!buffer) {
		goto out;
	    }
	}
	strcpy (buffer + pathlen, ent->d_name);
	f = zfile_open (buffer, "rb");
	if (!f)
	    continue;

	zfile_fseek (f, 0, SEEK_END);
	size = zfile_ftell (f);

	/* Weed out too-large files to save time.  */
	if (size > 1024 * 1024) {
	    zfile_fclose (f);
	    continue;
	}
	zfile_fseek (f, 0, SEEK_SET);
	zfile_fread (data, 1, size, f);

	rd = getromdatabydata (data, size);
	zfile_fclose (f);

	/* Add encrypted ROMs even if we don't have the key yet.  */
	if (!rd && !is_encrypted_rom (data, size))
	    continue;

	if (rd && rd->type == ROMTYPE_KEY) {
	    if (addkey (data, size, ent->d_name))
		keys_added = 1;
	} else
	    romlist_add (buffer, rd, loc);
    }
    /* Now, if we added any keys, reexamine all encrypted ROMs.  */
    if (keys_added) {
	int i;
	for (i = 0; i < romlist_cnt; i++) {
	    struct romlist *rl = list_of_roms + i;
	    if (rl->rd)
		continue;
	    struct zfile *f = zfile_open (rl->path, "rb");
	    if (!f)
		continue;
	    rl->rd = getromdatabyzfile (f);
	    zfile_fclose (f);
	}
    }
    sort_romlist ();
    gui_romlist_changed ();

    free (data);
  out1:
    free (buffer);
  out:
    closedir (dir);
}
