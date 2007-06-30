/*
 * hyDDRe
 * $Id: sm.h,v 1.1.1.1 2006-03-31 19:00:44 root Exp $
 */

#ifndef __SM
#define __SM

namespace Sm
{

/*
 * sm
 *
 * @author: Matthieu Guillemot <g@npng.org>
 * @date: 09/06/2005
 * @version: 1.00
 *
 *
 * Lecteur de fichiers SM pour hyDDRe.
 *
 *
 * Format SM :
 *   http://www.stepmania.com/stepmania/viewfile.php?file=README-FIRST.html#The%20.SM%20file%20format
 *
 */


/*** Types exportés ***/

struct BPMchange
{
  float beat;
  float bpm;
};

struct Stop
{
  float beat;
  float sec;
};

struct BGchange
{
  float beat;
  char *bg;
};

enum NotesType { DANCE_SINGLE, DANCE_DOUBLE, OTHER };

enum DifficultyClass { BEGINNER, EASY, MEDIUM, HARD, CHALLENGE };

enum Direction { LEFT_A = 0, DOWN_A = 1, UP_A = 2, RIGHT_A = 3, LEFT_A_2 = 4, DOWN_A_2 = 5, UP_A_2 = 6, RIGHT_A_2 = 7 };

enum TapType { NONE = 0, TAP = 1, BEGIN_HOLD = 2, END_HOLD = 3 };

struct Tap
{
  TapType tap[8];
};

struct Measure
{
  int  notes_nb;
  Tap *notes;
};

struct Level
{
  NotesType       notestype;
  int             nbtaps;
  char           *description;
  DifficultyClass difficultyclass;
  int             difficultymeter;
  float           difficultygrade[5];
  int             measure_nb;
  Measure        *measure;
};

struct SMfile
{
  char      *title;
  char      *subtitle;
  char      *artist;
  char      *titletranslit;
  char      *subtitletranslit;
  char      *artisttranslit;
  char      *credit;
  char      *banner;
  char      *background;
  char      *cdtitle;
  char      *music;
  float      offset;
  float      samplestart;
  float      samplelength;
  int        selectable;
  int        bpms_nb;
  BPMchange *bpms;
  int        stops_nb;
  Stop      *stops;
//int        bgchange_nb; // désactivés pour le moment
//BGchange  *bgchange;    // désactivés pour le moment
  int        level_nb;
  Level     *level;
};


/*** Fonctions exportées ***/

bool load_sm(SMfile *sm, char filename[]);

}

#endif // __SM
