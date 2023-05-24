//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://www.vreng.enst.fr/
//
// Copyright (C) 1997-2021 Philippe Dax
// Telecom-Paris (Ecole Nationale Superieure des Telecommunications)
//
// VREng is a free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public Licence as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// VREng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//---------------------------------------------------------------------------
#include "vreng.hpp"
#include "pref.hpp"
#include "env.hpp"	// ::g.env
#include "vac.hpp"	// Vac
#include "theme.hpp"	// Theme
#include "file.hpp"	// open, close
#include "str.hpp"	// stringcmp
#include "ubit/uconf.hpp" // Uconf::

#include "prefs.t"	// prefs config


static const char HELPSTRING[] = "\
Usage: vreng [options]\n\
where options are:\n\
-a, --avatar model		Avatar model [guy | humanoid | human | box]\n\
-b, --bbox	 		Draw bounding-boxes\n\
-d, --debug mask 		Debug mask [1,3,7,15,31,63, ...]\n\
-f, --frames rate		Max frames per second [1..255]\n\
-g, --nogravity			Without gravity\n\
-h, --help			Help message and exit\n\
-i, --infogl			Infos OpenGL [show]\n\
-k, --keepcache			Do not keep *.vre in cache, force to download\n\
-l, --listcache			List the cache and exit\n\
-n, --number number		Number of simultaneous http threads [1..7]\n\
-p, --pseudo name		Your pseudoname\n\
-q, --quality			high OpenGL quality, if you have a graphic 3D card\n\
-r, --refresh			Refresh the cache\n\
-s, --silent			No sounds effects\n\
-t, --trace			Trace for debugging\n\
-u, --universe url		Universe url of httpd server\n\
-v, --version			Version number\n\
-w, --world url			World url [http://, file://]\n\
-2, --fullscreen		Screen double size\n\
-3, --thirdview			Thirdperson view\n\
-A, --address group/port/ttl	Multicast address (deprecated)\n\
-C, --clean			Clean cache\n\
-D, --debugger			Debugging lldb or gdb (reserved for vr)\n\
-E, --expand			Expand palettes (GUI)\n\
-F, --fillcache			Fill the cache artificialy\n\
-L, --loghttpd			Logging more httpd client infos\n\
-M, --multicast			MBone IP Multicast mode\n\
-N, --nopersist			No persistency (without MySql | Sqlite | Postgres)\n\
-P, --progress			Progression indicators [show]\n\
-Q, --sqltrace table		Sql trace table\n\
-R, --reflector			Reflector unicast/multicast mode\n\
-S, --stats			Stats when quiting [show]\n\
-T, --timetolive days		Cache time in days\n\
-U, --ubit			Show ubit help\n\
";


Pref::Pref()
{
  ::g.version = strdup(PACKAGE_VERSION);
  ::g.url = NULL;
  ::g.urlpfx = NULL;
  ::g.server = NULL;
  ::g.universe = NULL;
  ::g.user = NULL;
  ::g.channel = NULL;

  debug = 0;

  // options true by default
  gravity = true;
  cpulimit = true;
  keep = true;
  silent = true;
  reflector = true;

  // options false by default
  dbg = false;
  infogl = false;
  quality3D = false;
  nopersist = false;
  stats = false;
  progress = false;
  expand = false;
  bbox = false;
  dbgtrace = false;
  refresh = false;
  loghttpd = false;
  tview = false;
  sql = false;

  // options with default values
  width3D  = DEF_WIDTH3D;
  height3D = DEF_HEIGHT3D;
  maxsimcon = DEF_MAXSIMCON;
  maxfps = DEF_RATE;
  frame_delay = 1000000 / DEF_RATE;	// 20 ms
  cachetime = 3600 * 24 * 3;	// 3 days

  my_avatar = NULL;
  my_vrestr = NULL;
  my_widthstr = NULL;
  my_depthstr = NULL;
  my_heightstr = NULL;
  my_mapfrontstr = NULL;
  my_mapbackstr = NULL;
  my_hoststr = NULL;
  my_webstr = NULL;
  my_facestr = NULL;
  my_sexstr = NULL;
  my_headstr = NULL;
  my_skinstr = NULL;
  my_buststr = NULL;
  my_colorstr = NULL;
  my_bapsstr = NULL;
  my_skinf = NULL;
  my_skinb = NULL;
  httpproxystr = NULL;
  noproxystr = NULL;
  mcastproxystr = NULL;
  sqltable = NULL;
}

void Pref::init(int argc, char **argv, const char* pref_file)
{
  initPrefs(pref_file);  // ::g.env.prefs());
  parse(argc, argv);
  trace(DBG_INIT, "Pref initialized");
}

/** parses options in command line */
void Pref::parse(int argc, char **argv)
{
  extern char *optarg;
  int c, v;
  bool helpx = false;

#if HAVE_GETOPT_LONG
  static struct option longopts[] = {
    {"avatar",     1, 0, 'a'},
    {"bbox",       0, 0, 'b'},
    {"debug",      1, 0, 'd'},
    {"frames",     1, 0, 'f'},
    {"nogravity",  0, 0, 'g'},
    {"help",       0, 0, 'h'},
    {"infogl",     0, 0, 'i'},
    {"keepcache",  0, 0, 'k'},
    {"listcache",  0, 0, 'l'},
    {"number",     1, 0, 'n'},
    {"pseudo",     1, 0, 'p'},
    {"quality",    0, 0, 'q'},
    {"refresh",    0, 0, 'r'},
    {"silent",     0, 0, 's'},
    {"trace",      0, 0, 't'},
    {"universe",   1, 0, 'u'},
    {"version",    0, 0, 'v'},
    {"world",      1, 0, 'w'},
    {"fullscreen", 0, 0, '2'},
    {"thirdview",  0, 0, '3'},
    {"address",    1, 0, 'A'},
    {"clean",      0, 0, 'C'},
    {"expand",     0, 0, 'E'},
    {"fillcache",  0, 0, 'F'},
    {"loghttpd",   0, 0, 'L'},
    {"multicast",  0, 0, 'M'},
    {"nopersist",  0, 0, 'N'},
    {"progress",   0, 0, 'P'},
    {"sqltrace",   1, 0, 'Q'},
    {"stats",      0, 0, 'S'},
    {"reflector",  0, 0, 'R'},
    {"timetolive", 1, 0, 'T'},
    {"ubit",       0, 0, 'U'},
    {0,0,0,0}
  };
  while ((c = getopt_long(argc, argv, "bghiklqrstv23CDEFLMNPRSUa:d:f:n:p:u:w:A:Q:T:", longopts, NULL))
#else
  while ((c = getopt(argc, argv, "-bghiklqrstvx23CDEFLMNPRSUa:d:f:n:p:u:w:A:Q:T:"))
#endif
   != -1) {

    switch (c) {
      case '-':
        echo("-- long options not available, use - short options");
        break;
      case 'a':
        my_avatar = strdup(optarg);
        break;
      case 'b':
        bbox = true;
        break;
      case 'd':
        dbg = atoi(optarg);
        debug = atoi(optarg);
        break;
      case 'f':
        if (! optarg) {
          frame_delay = 0;
          cpulimit = false;
        }
        maxfps = atoi(optarg);
        if (maxfps == 0) {
          maxfps = DEF_MAXFRAMES;
          frame_delay = 0;
          cpulimit = false;
        }
        else {
          frame_delay = 1000000 / maxfps;
          cpulimit = true;
        }
        break;
      case 'g':
        gravity = false;
        break;
      case 'h':
        printf("%s\n", HELPSTRING);
        exit(0);
      case 'i':
        infogl = true;
        break;
      case 'k':
        keep = false;
        break;
      case 'l':
        ::g.env.listCache();
        exit(0);
      case 'n':
        maxsimcon = atoi(optarg);
        if (maxsimcon >= DEF_MAXSIMCON) {
          error("too many simultaneous connections, set to %d", DEF_MAXSIMCON);
          maxsimcon = DEF_MAXSIMCON;
        }
        break;
      case 'p':
        ::g.user = strdup(optarg);
        break;
      case 'q':		// quality and performance
        quality3D = true;
        frame_delay = 0;
        cpulimit = false;
        break;
      case 'r':
        refresh = true;
        break;
      case 's':
        silent = false;
        break;
      case 't':
        dbgtrace = true;
        break;
      case 'u':
        ::g.universe = strdup(optarg);
        break;
      case 'v':
        printf("%s\n", PACKAGE_VERSION);
        exit(0);
      case 'w':
        ::g.url = strdup(optarg);
	{
          char chanstr[CHAN_LEN];
          memset(chanstr, 0, sizeof(chanstr));
	  Vac *vac = Vac::current();
          //dax FIXME segfault: vac->resolveWorldUrl(::g.url, chanstr);
          ::g.channel = strdup(chanstr);
	}
        break;
      case 'x':
        helpx = true;
        break;
      case '2':
        width3D = 1180;
        height3D = 656;
        break;
      case '3':
        tview = true;
        break;
      case 'A':
        ::g.channel = strdup(optarg);
        reflector = false;
        break;
      case 'B':
#if 0 //obsoleted
        switch (*optarg) {
        case 'b': gui_skin = Theme::BLACK; break; // black
        case 'g': gui_skin = Theme::GREY; break; // grey
        case 'y': gui_skin = Theme::YELLOW; break; // yellow
        case 'w': gui_skin = Theme::WHITE; break; // white
        case 'W': gui_skin = Theme::WOOD; break; // wood
        }
#endif
        break;
      case 'C':
        ::g.env.cleanCacheByTime(0L);
        break;
      case 'D':
        break;
      case 'E':
        expand = true;
        break;
      case 'F':
        ::g.env.fillCache();
        break;
      case 'L':
        loghttpd = true;
        break;
      case 'M':
        reflector = false;
        break;
      case 'N':
        nopersist = true;
        break;
      case 'P':
        progress = true;
        break;
      case 'Q':
        sql = true;
        sqltable = strdup(optarg);
        break;
      case 'R':
        reflector = true;
        break;
      case 'S':
        stats = true;
        break;
      case 'T':
        v = atoi(optarg);
	if (v < 0 || v > 365) {
          v = 3;	// 3 days
        }
        cachetime = v * 3600 * 24;
        break;
      case 'U':
        UConf::printHelp();
        exit(0);
        break;
    }
  }

  // username
  if (::g.user == NULL) {
    struct passwd *pwd = getpwuid(getuid());
    if (pwd) {
      ::g.user = strdup(pwd->pw_name);	// login name
    }
    else {
      ::g.user = strdup("nobody");
    }
  }

  /////////
  // urlpfx
  //
  char *pfx = new char[URL_LEN];

  if (strncmp(DEF_URL_PFX, "~%s", 3) == 0) {
    strcat(pfx, "~");			// ~/public_html/ or ~/Sites/
    strcat(pfx, ::g.user);		// loginname
    strcat(pfx, &DEF_URL_PFX[3]);	// path (htdoc)
  }
  else {
    strcpy(pfx, DEF_URL_PFX);
  }
  ::g.urlpfx = strdup(pfx);
  //echo("::g.urlpfx: %s", ::g.urlpfx);

  /////////
  // url
  //
  char *url = new char[URL_LEN];

  if (::g.url == NULL) {
    if (::g.universe == NULL) {
      sprintf(url, "http://%s/%s%s", DEF_HTTP_SERVER, ::g.urlpfx, DEF_URL_WORLD);
    }
    else {
      sprintf(url, "%s%s%s", ::g.universe, "", DEF_URL_WORLD);
    }
    ::g.url = strdup(url);
  }
  //echo("::g.url: %s", ::g.url);

  /////////
  // server
  //
  char *srv = new char[URL_LEN];

  if (::g.universe == NULL) {
    sprintf(srv, "%s", DEF_HTTP_SERVER);
  }
  else {
    char *p1, *p2;
    strcpy(srv, ::g.universe);
    p1 = strchr(srv, '/');
    p1++;
    p1 = strchr(p1, '/');
    p2 = ++p1;
    p1 = strchr(p1, '/');
    *p1 = '\0';
    strcpy(srv, p2); 
  }
  ::g.server = strdup(srv);
  //echo("::g.server: %s", ::g.server);

  //echo("url: %s", ::g.url);
  //echo("server: %s", ::g.server);
  //echo("universe: %s", ::g.universe);
  //echo("pfx: %s", ::g.urlpfx);

  // skins
  char *urlskinf = new char[URL_LEN];
  char *urlskinb = new char[URL_LEN];

  if (::g.universe == NULL) {
    sprintf(urlskinf, "http://%s/%s%s", DEF_HTTP_SERVER, ::g.urlpfx, DEF_URL_FRONT);
    sprintf(urlskinb, "http://%s/%s%s", DEF_HTTP_SERVER, ::g.urlpfx, DEF_URL_BACK);
  }
  else {
    sprintf(urlskinf, "%s%s%s", ::g.universe, "", DEF_URL_FRONT);
    sprintf(urlskinb, "%s%s%s", ::g.universe, "", DEF_URL_BACK);
  }
  my_skinf = strdup(urlskinf);
  my_skinb = strdup(urlskinb);

  if (::g.channel == NULL) {
    ::g.channel = strdup(DEF_VRENG_CHANNEL);
  }

  // checks ubit options
  if (helpx) {
    argc++;
    argv++;
    *argv = new char[10];
    strcpy(*argv, "--help-x");
  }
}

void Pref::initPrefs(const char* pref_file)
{
  FILE *fp;
  char *p1, *p2, buf[BUFSIZ];

  File *filein = new File();
  File *fileout = new File();
  if ((fp = filein->open(pref_file, "r")) == NULL) {
    if ((fp = fileout->open(pref_file, "w")) == NULL) {
      perror("can't create prefs");
      return;
    }
    fputs(def_prefs, fp);
    fileout->close();
    delete fileout;
    if ((fp = filein->open(pref_file, "r")) == NULL) {
      error("can't read prefs");
      return;
    }
  }

  // read prefs file
  while (fgets(buf, sizeof(buf), fp)) {
    if (*buf == '#' || *buf == '\n') {
      continue;
    }
    buf[strlen(buf) - 1] = '\0';
    p1 = strtok(buf, " \t=");
    p2 = strtok(NULL, " \t#");
    if (stringcmp(p1, "world") == 0) {
      my_vrestr = new char[strlen(p2) + 1];
      strcpy(my_vrestr, p2);
      trace(DBG_INIT, "vre = %s", p2);
    }
    else if (stringcmp(p1, "width") == 0) {
      my_widthstr = new char[strlen(p2) + 1];
      strcpy(my_widthstr, p2);
      trace(DBG_INIT, "width = %s", p2);
    }
    else if (stringcmp(p1, "depth") == 0) {
      my_depthstr = new char[strlen(p2) + 1];
      strcpy(my_depthstr, p2);
      trace(DBG_INIT, "depth = %s", p2);
    }
    else if (stringcmp(p1, "height") == 0) {
      my_heightstr = new char[strlen(p2) + 1];
      strcpy(my_heightstr, p2);
      trace(DBG_INIT, "height = %s", p2);
    }
    else if (stringcmp(p1, "mapfront") == 0 || stringcmp(p1, "mapface") == 0) {
      my_mapfrontstr = new char[strlen(p2) + 1];
      strcpy(my_mapfrontstr, p2);
    }
    else if (stringcmp(p1, "mapback") == 0) {
      my_mapbackstr = new char[strlen(p2) + 1];
      strcpy(my_mapbackstr, p2);
    }
    else if (stringcmp(p1, "host") == 0) {
      my_hoststr = new char[strlen(p2) + 1];
      strcpy(my_hoststr, p2);
    }
    else if (stringcmp(p1, "web") == 0) {
      my_webstr = new char[strlen(p2) + 1];
      strcpy(my_webstr, p2);
    }
    else if (stringcmp(p1, "model") == 0) {
      my_avatar = new char[strlen(p2) + 1];
      strcpy(my_avatar, p2);
      trace(DBG_INIT, "model = %s", p2);
    }
    else if (stringcmp(p1, "face") == 0) {
      my_facestr = new char[strlen(p2) + 1];
      strcpy(my_facestr, p2);
    }
    else if (stringcmp(p1, "sex") == 0) {
      my_sexstr = new char[strlen(p2) + 1];
      strcpy(my_sexstr, p2);
    }
    else if (stringcmp(p1, "head") == 0) {
      my_headstr = new char[strlen(p2) + 1];
      strcpy(my_headstr, p2);
    }
    else if (stringcmp(p1, "skin") == 0) {
      my_skinstr = new char[strlen(p2) + 1];
      strcpy(my_skinstr, p2);
    }
    else if (stringcmp(p1, "bust") == 0) {
      my_buststr = new char[strlen(p2) + 1];
      strcpy(my_buststr, p2);
    }
    else if (stringcmp(p1, "color") == 0) {
      my_colorstr = new char[strlen(p2) + 1];
      strcpy(my_colorstr, p2);
      trace(DBG_INIT, "color = %s", p2);
    }
    else if (stringcmp(p1, "baps") == 0) {
      my_bapsstr = new char[strlen(p2) + 1];
      strcpy(my_bapsstr, p2);
      trace(DBG_INIT, "baps = %s", p2);
    }
    else if (stringcmp(p1, "http_proxy") == 0) {
      httpproxystr = new char[strlen(p2) + 1];
      strcpy(httpproxystr, p2);
    }
    else if (stringcmp(p1, "no_proxy") == 0) {
      noproxystr = new char[strlen(p2) + 1];
      strcpy(noproxystr, p2);
    }
    else if (stringcmp(p1, "mcast_proxy") == 0) {
      mcastproxystr = new char[strlen(p2) + 1];
      strcpy(mcastproxystr, p2);
    }
  } //eof

  filein->close();
  delete filein;
}
