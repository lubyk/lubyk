#line 1 "classes/command.rl"
#include "command.h"
#include "rubyk.h"
#include "class.h"
#include "mutex.h"

//#define DEBUG_PARSER


#line 11 "classes/command.cpp"
static const char _command_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 7, 1, 8, 1, 13, 1, 
	14, 1, 15, 1, 17, 1, 18, 1, 
	19, 2, 1, 6, 2, 1, 11, 2, 
	1, 12, 2, 2, 14, 2, 2, 15, 
	2, 5, 9, 2, 5, 10, 2, 13, 
	17, 2, 14, 17, 2, 15, 17, 2, 
	16, 17, 3, 1, 11, 17, 3, 1, 
	12, 17, 3, 2, 14, 17, 3, 2, 
	15, 17, 3, 2, 16, 17, 3, 5, 
	10, 0
};

static const short _command_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 22, 34, 
	37, 45, 47, 51, 54, 61, 63, 70, 
	74, 76, 82, 82, 90, 97, 99, 104, 
	109, 109, 111, 119, 121, 128, 142, 146, 
	147, 155, 157, 160, 162, 173, 178, 182, 
	190, 198, 200, 204, 207, 214, 216, 223, 
	227, 229, 235, 235, 243, 250, 252, 257, 
	262, 262, 264, 272, 274, 281, 289, 291, 
	294, 296, 307, 315, 317, 321, 324, 331, 
	333, 340, 341, 343, 349, 349, 357, 364, 
	366, 371, 376, 376, 378, 386, 388, 395, 
	401, 403, 409, 413, 414, 426, 434, 436, 
	440, 443, 450, 452, 459, 463, 465, 471, 
	471, 479, 486, 488, 493, 498, 498, 500, 
	508, 510, 517, 518, 526
};

static const char _command_trans_keys[] = {
	9, 10, 32, 35, 65, 90, 97, 122, 
	9, 10, 32, 10, 46, 95, 48, 57, 
	65, 90, 97, 122, 97, 122, 9, 10, 
	32, 35, 40, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 35, 34, 41, 43, 
	45, 48, 57, 97, 122, 34, 92, 9, 
	32, 41, 44, 9, 32, 44, 9, 32, 
	34, 43, 45, 48, 57, 48, 57, 9, 
	32, 41, 44, 46, 48, 57, 9, 10, 
	32, 35, 48, 57, 9, 32, 41, 44, 
	48, 57, 58, 95, 48, 57, 65, 90, 
	97, 122, 9, 32, 34, 43, 45, 48, 
	57, 34, 92, 9, 32, 41, 97, 122, 
	9, 32, 41, 97, 122, 48, 57, 9, 
	32, 41, 46, 48, 57, 97, 122, 48, 
	57, 9, 32, 41, 48, 57, 97, 122, 
	9, 10, 32, 40, 46, 47, 61, 95, 
	48, 57, 65, 90, 97, 122, 9, 32, 
	47, 61, 47, 9, 32, 43, 45, 48, 
	57, 97, 122, 48, 57, 46, 48, 57, 
	97, 122, 9, 10, 32, 35, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 62, 
	65, 90, 9, 32, 65, 90, 40, 95, 
	48, 57, 65, 90, 97, 122, 34, 41, 
	43, 45, 48, 57, 97, 122, 34, 92, 
	9, 32, 41, 44, 9, 32, 44, 9, 
	32, 34, 43, 45, 48, 57, 48, 57, 
	9, 32, 41, 44, 46, 48, 57, 9, 
	10, 32, 35, 48, 57, 9, 32, 41, 
	44, 48, 57, 58, 95, 48, 57, 65, 
	90, 97, 122, 9, 32, 34, 43, 45, 
	48, 57, 34, 92, 9, 32, 41, 97, 
	122, 9, 32, 41, 97, 122, 48, 57, 
	9, 32, 41, 46, 48, 57, 97, 122, 
	48, 57, 9, 32, 41, 48, 57, 97, 
	122, 9, 32, 43, 45, 48, 57, 97, 
	122, 48, 57, 46, 48, 57, 97, 122, 
	9, 10, 32, 35, 95, 48, 57, 65, 
	90, 97, 122, 34, 41, 43, 45, 48, 
	57, 97, 122, 34, 92, 9, 32, 41, 
	44, 9, 32, 44, 9, 32, 34, 43, 
	45, 48, 57, 48, 57, 9, 32, 41, 
	44, 46, 48, 57, 10, 48, 57, 9, 
	32, 41, 44, 48, 57, 58, 95, 48, 
	57, 65, 90, 97, 122, 9, 32, 34, 
	43, 45, 48, 57, 34, 92, 9, 32, 
	41, 97, 122, 9, 32, 41, 97, 122, 
	48, 57, 9, 32, 41, 46, 48, 57, 
	97, 122, 48, 57, 9, 32, 41, 48, 
	57, 97, 122, 43, 45, 48, 57, 97, 
	122, 48, 57, 9, 32, 47, 61, 48, 
	57, 9, 32, 47, 61, 62, 9, 10, 
	32, 35, 40, 95, 48, 57, 65, 90, 
	97, 122, 34, 41, 43, 45, 48, 57, 
	97, 122, 34, 92, 9, 32, 41, 44, 
	9, 32, 44, 9, 32, 34, 43, 45, 
	48, 57, 48, 57, 9, 32, 41, 44, 
	46, 48, 57, 9, 10, 32, 35, 48, 
	57, 9, 32, 41, 44, 48, 57, 58, 
	95, 48, 57, 65, 90, 97, 122, 9, 
	32, 34, 43, 45, 48, 57, 34, 92, 
	9, 32, 41, 97, 122, 9, 32, 41, 
	97, 122, 48, 57, 9, 32, 41, 46, 
	48, 57, 97, 122, 48, 57, 9, 32, 
	41, 48, 57, 97, 122, 10, 9, 10, 
	32, 35, 65, 90, 97, 122, 0
};

static const char _command_single_lengths[] = {
	0, 4, 3, 1, 2, 0, 6, 3, 
	4, 2, 4, 3, 5, 0, 5, 4, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 8, 4, 1, 
	4, 0, 1, 0, 5, 3, 2, 2, 
	4, 2, 4, 3, 5, 0, 5, 4, 
	0, 4, 0, 2, 5, 2, 3, 3, 
	0, 0, 4, 0, 3, 4, 0, 1, 
	0, 5, 4, 2, 4, 3, 5, 0, 
	5, 1, 0, 4, 0, 2, 5, 2, 
	3, 3, 0, 0, 4, 0, 3, 2, 
	0, 4, 4, 1, 6, 4, 2, 4, 
	3, 5, 0, 5, 4, 0, 4, 0, 
	2, 5, 2, 3, 3, 0, 0, 4, 
	0, 3, 1, 4, 0
};

static const char _command_range_lengths[] = {
	0, 2, 0, 0, 3, 1, 3, 0, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 3, 0, 0, 
	2, 1, 1, 1, 3, 1, 1, 3, 
	2, 0, 0, 0, 1, 1, 1, 0, 
	1, 1, 0, 3, 1, 0, 1, 1, 
	0, 1, 2, 1, 2, 2, 1, 1, 
	1, 3, 2, 0, 0, 0, 1, 1, 
	1, 0, 1, 1, 0, 3, 1, 0, 
	1, 1, 0, 1, 2, 1, 2, 2, 
	1, 1, 0, 0, 3, 2, 0, 0, 
	0, 1, 1, 1, 0, 1, 1, 0, 
	3, 1, 0, 1, 1, 0, 1, 2, 
	1, 2, 0, 2, 0
};

static const short _command_index_offsets[] = {
	0, 0, 7, 11, 13, 19, 21, 31, 
	35, 42, 45, 50, 54, 61, 63, 70, 
	75, 77, 83, 84, 90, 97, 100, 105, 
	110, 111, 113, 120, 122, 128, 140, 145, 
	147, 154, 156, 159, 161, 170, 175, 179, 
	185, 192, 195, 200, 204, 211, 213, 220, 
	225, 227, 233, 234, 240, 247, 250, 255, 
	260, 261, 263, 270, 272, 278, 285, 287, 
	290, 292, 301, 308, 311, 316, 320, 327, 
	329, 336, 338, 340, 346, 347, 353, 360, 
	363, 368, 373, 374, 376, 383, 385, 391, 
	396, 398, 404, 409, 411, 421, 428, 431, 
	436, 440, 447, 449, 456, 461, 463, 469, 
	470, 476, 483, 486, 491, 496, 497, 499, 
	506, 508, 514, 516, 523
};

static const unsigned char _command_indicies[] = {
	1, 2, 1, 3, 4, 5, 0, 1, 
	2, 1, 0, 2, 3, 6, 4, 4, 
	4, 4, 0, 7, 0, 8, 9, 8, 
	10, 11, 7, 7, 7, 7, 0, 12, 
	12, 3, 0, 13, 14, 15, 15, 16, 
	17, 0, 19, 20, 18, 21, 21, 22, 
	23, 0, 24, 24, 25, 0, 25, 25, 
	13, 15, 15, 16, 0, 16, 0, 21, 
	21, 22, 23, 26, 16, 0, 27, 28, 
	27, 29, 0, 30, 0, 21, 21, 22, 
	23, 30, 0, 18, 31, 17, 17, 17, 
	17, 0, 32, 32, 33, 34, 34, 35, 
	0, 37, 38, 36, 39, 39, 40, 41, 
	0, 42, 42, 14, 17, 0, 36, 35, 
	0, 39, 39, 40, 43, 35, 41, 0, 
	44, 0, 39, 39, 40, 44, 41, 0, 
	45, 46, 45, 47, 48, 49, 50, 5, 
	5, 5, 5, 0, 51, 51, 52, 53, 
	0, 54, 0, 54, 54, 55, 55, 56, 
	57, 0, 56, 0, 58, 56, 0, 57, 
	0, 59, 60, 59, 61, 57, 57, 57, 
	57, 0, 62, 62, 63, 64, 0, 62, 
	62, 64, 0, 65, 64, 64, 64, 64, 
	0, 66, 67, 68, 68, 69, 70, 0, 
	72, 73, 71, 74, 74, 75, 76, 0, 
	77, 77, 78, 0, 78, 78, 66, 68, 
	68, 69, 0, 69, 0, 74, 74, 75, 
	76, 79, 69, 0, 80, 81, 80, 82, 
	0, 83, 0, 74, 74, 75, 76, 83, 
	0, 71, 84, 70, 70, 70, 70, 0, 
	85, 85, 86, 87, 87, 88, 0, 90, 
	91, 89, 92, 92, 93, 94, 0, 95, 
	95, 67, 70, 0, 89, 88, 0, 92, 
	92, 93, 96, 88, 94, 0, 97, 0, 
	92, 92, 93, 97, 94, 0, 63, 63, 
	98, 98, 99, 100, 0, 99, 0, 101, 
	99, 0, 100, 0, 102, 103, 102, 104, 
	100, 100, 100, 100, 0, 105, 106, 107, 
	107, 108, 109, 0, 111, 112, 110, 113, 
	113, 114, 115, 0, 116, 116, 117, 0, 
	117, 117, 105, 107, 107, 108, 0, 108, 
	0, 113, 113, 114, 115, 118, 108, 0, 
	119, 0, 120, 0, 113, 113, 114, 115, 
	120, 0, 110, 121, 109, 109, 109, 109, 
	0, 122, 122, 123, 124, 124, 125, 0, 
	127, 128, 126, 129, 129, 130, 131, 0, 
	132, 132, 106, 109, 0, 126, 125, 0, 
	129, 129, 130, 133, 125, 131, 0, 134, 
	0, 129, 129, 130, 134, 131, 0, 135, 
	135, 136, 137, 0, 136, 0, 138, 138, 
	139, 140, 136, 0, 141, 141, 52, 142, 
	0, 63, 0, 143, 144, 143, 145, 146, 
	137, 137, 137, 137, 0, 147, 148, 149, 
	149, 150, 151, 0, 153, 154, 152, 155, 
	155, 156, 157, 0, 158, 158, 159, 0, 
	159, 159, 147, 149, 149, 150, 0, 150, 
	0, 155, 155, 156, 157, 160, 150, 0, 
	161, 162, 161, 163, 0, 164, 0, 155, 
	155, 156, 157, 164, 0, 152, 165, 151, 
	151, 151, 151, 0, 166, 166, 167, 168, 
	168, 169, 0, 171, 172, 170, 173, 173, 
	174, 175, 0, 176, 176, 148, 151, 0, 
	170, 169, 0, 173, 173, 174, 177, 169, 
	175, 0, 178, 0, 173, 173, 174, 178, 
	175, 0, 180, 179, 1, 2, 1, 3, 
	4, 5, 0, 181, 0
};

static const char _command_trans_targs_wi[] = {
	0, 2, 115, 3, 4, 29, 5, 6, 
	7, 115, 3, 8, 7, 9, 15, 13, 
	14, 19, 9, 10, 18, 11, 15, 12, 
	11, 12, 16, 7, 115, 3, 17, 20, 
	20, 21, 25, 26, 21, 22, 24, 23, 
	15, 19, 23, 27, 28, 30, 115, 66, 
	87, 31, 37, 30, 31, 37, 32, 33, 
	34, 36, 35, 7, 115, 3, 38, 61, 
	39, 40, 41, 47, 45, 46, 51, 41, 
	42, 50, 43, 47, 44, 43, 44, 48, 
	7, 115, 3, 49, 52, 52, 53, 57, 
	58, 53, 54, 56, 55, 47, 51, 55, 
	59, 60, 62, 63, 65, 64, 7, 115, 
	3, 67, 73, 71, 72, 77, 67, 68, 
	76, 69, 73, 70, 69, 70, 74, 115, 
	75, 78, 78, 79, 83, 84, 79, 80, 
	82, 81, 73, 77, 81, 85, 86, 88, 
	89, 92, 90, 31, 91, 90, 91, 7, 
	115, 3, 93, 94, 100, 98, 99, 104, 
	94, 95, 103, 96, 100, 97, 96, 97, 
	101, 7, 115, 3, 102, 105, 105, 106, 
	110, 111, 106, 107, 109, 108, 100, 104, 
	108, 112, 113, 114, 116, 0
};

static const char _command_trans_actions_wi[] = {
	21, 0, 19, 0, 1, 1, 7, 1, 
	37, 70, 37, 3, 0, 0, 0, 1, 
	1, 1, 1, 0, 1, 40, 40, 40, 
	0, 0, 1, 17, 52, 17, 1, 5, 
	0, 0, 1, 1, 1, 0, 1, 43, 
	43, 78, 0, 1, 1, 25, 74, 3, 
	25, 25, 25, 0, 0, 0, 0, 1, 
	1, 1, 11, 31, 62, 31, 0, 0, 
	1, 7, 0, 0, 1, 1, 1, 1, 
	0, 1, 40, 40, 40, 0, 0, 1, 
	13, 46, 13, 1, 5, 0, 0, 1, 
	1, 1, 0, 1, 43, 43, 78, 0, 
	1, 1, 1, 1, 1, 11, 28, 58, 
	28, 0, 0, 1, 1, 1, 1, 0, 
	1, 40, 40, 40, 0, 0, 1, 55, 
	1, 5, 0, 0, 1, 1, 1, 0, 
	1, 43, 43, 78, 0, 1, 1, 1, 
	1, 1, 9, 9, 9, 0, 0, 34, 
	66, 34, 3, 0, 0, 1, 1, 1, 
	1, 0, 1, 40, 40, 40, 0, 0, 
	1, 15, 49, 15, 1, 5, 0, 0, 
	1, 1, 1, 0, 1, 43, 43, 78, 
	0, 1, 1, 0, 23, 0
};

static const char _command_eof_actions[] = {
	0, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 0, 0, 0
};

static const int command_start = 1;
static const int command_first_final = 115;

static const int command_en_eat_line = 114;
static const int command_en_main = 1;

#line 11 "classes/command.rl"


void Command::initialize()
{
  int cs;
  
  mAction = NO_ACTION;
  mServer = NULL;
  mQuit   = false;
  mTokenIndex = 0;
  mSilent = false;
  mThread = NULL;
  
  clear();
  
  
#line 329 "classes/command.cpp"
	{
	cs = command_start;
	}
#line 27 "classes/command.rl"
  mCurrentState = cs;
}

int Command::do_listen()
{
  char buffer[1024];
  char * line = buffer;
  
  // set thread priority to normal
  mServer->normal_priority();
  
  if (!mSilent)
    *mOutput << "Welcome to rubyk !\n\n";
  clear();

  while(!mQuit && getline(&line,1023)) {
    parse(line);
    parse("\n");
    if (!mQuit)
      saveline(line);
    freeline(line);
  }
  return 0; // thread return value
}

void Command::parse(const std::string& pStr)
{
  const char *p  = pStr.data(); // data pointer
  const char *pe = p + pStr.size(); // past end
  const char *eof = NULL;  // FIXME: this should be set to 'pe' on the last pStr block...
  int cs = mCurrentState;        // restore machine state
  
  
#line 367 "classes/command.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _command_indicies[_trans];
	cs = _command_trans_targs_wi[_trans];

	if ( _command_trans_actions_wi[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions_wi[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 60 "classes/command.rl"
	{
      if (mTokenIndex >= MAX_TOKEN_SIZE) {
        std::cerr << "Buffer overflow !" << std::endl;
        // stop parsing
        return;
      }
#ifdef DEBUG_PARSER
      printf("_%c_",(*p));
#endif
      mToken[mTokenIndex] = (*p); /* append */
      mTokenIndex++;     
    }
	break;
	case 1:
#line 73 "classes/command.rl"
	{ set_from_token(mVariable);}
	break;
	case 2:
#line 75 "classes/command.rl"
	{ set_from_token(mMethod);}
	break;
	case 3:
#line 77 "classes/command.rl"
	{ set_from_token(mKey);}
	break;
	case 4:
#line 79 "classes/command.rl"
	{ set_from_token(mClass);}
	break;
	case 5:
#line 81 "classes/command.rl"
	{ set_from_token(mValue);}
	break;
	case 6:
#line 83 "classes/command.rl"
	{ mFrom = mVariable; }
	break;
	case 7:
#line 85 "classes/command.rl"
	{
      set_from_token(mValue);
      mFromPort = atoi(mValue.c_str());
    }
	break;
	case 8:
#line 90 "classes/command.rl"
	{
      set_from_token(mValue);
      mToPort = atoi(mValue.c_str());
    }
	break;
	case 9:
#line 95 "classes/command.rl"
	{ 
      add_value_from_token(); 
    }
	break;
	case 10:
#line 99 "classes/command.rl"
	{ set_parameter(mKey, mValue); }
	break;
	case 11:
#line 101 "classes/command.rl"
	{
      mTo   = mVariable;
      create_link();
    }
	break;
	case 12:
#line 106 "classes/command.rl"
	{
      mTo   = mVariable;
      remove_link();
    }
	break;
	case 13:
#line 111 "classes/command.rl"
	{ create_instance(); }
	break;
	case 14:
#line 113 "classes/command.rl"
	{ execute_method(); }
	break;
	case 15:
#line 115 "classes/command.rl"
	{ execute_class_method(); }
	break;
	case 16:
#line 117 "classes/command.rl"
	{ execute_command(); }
	break;
	case 17:
#line 121 "classes/command.rl"
	{
      if (!mQuit) {
        clear();
      }
    }
	break;
	case 18:
#line 126 "classes/command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 114; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
	case 19:
#line 135 "classes/command.rl"
	{ {cs = 1; goto _again;} }
	break;
#line 557 "classes/command.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _command_actions + _command_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 18:
#line 126 "classes/command.rl"
	{
      p--; // move back one char
      char error_buffer[10];
      snprintf(error_buffer, 9, "%s", p);
      *mOutput << "Syntax error near '" << error_buffer << "'." << std::endl;
      clear();
      {cs = 114; goto _again;} // eat the rest of the line and continue parsing
    }
	break;
#line 584 "classes/command.cpp"
		}
	}
	}

	_out: {}
	}
#line 185 "classes/command.rl"

//  printf("{%s}\n",p);
  mCurrentState = cs;
}


void Command::close()
{
  mQuit = true;
  if (mThread) {
    pthread_kill(mThread, SIGINT);
  }
}

void Command::set_from_token (std::string& pElem)
{
  mToken[mTokenIndex] = '\0';
#ifdef DEBUG_PARSER
  if (&pElem == &mValue) std::cout <<    "[val " << mToken << "]" << std::endl;
  if (&pElem == &mVariable) std::cout << "[var " << mToken << "]" << std::endl;
  if (&pElem == &mClass) std::cout <<    "[cla " << mToken << "]" << std::endl;
#endif
  pElem = mToken;
  mTokenIndex = 0;
}

void Command::add_value_from_token () 
{
  mParameters.add(mValue); // add to 'list' value
}

void Command::set_parameter  (const std::string& pKey, const std::string& pValue) 
{
  mParameters.set(pKey,pValue);
}

// FIXME: create_instance should run in server space with concurrency locks.
void Command::create_instance()
{
  mServer->lock();
  Node * node = mServer->create_instance(mVariable, mClass, mParameters, mOutput);
#ifdef DEBUG_PARSER
  std::cout << "NEW "<< mVariable << " = " << mClass << "(" << mParameters << ")";
#endif
  if (node) {
    if (!mSilent)
      *mOutput << node->inspect() << std::endl;
  } else {
    *mOutput << "Error" << std::endl;      
  }
  mServer->unlock();
}


void Command::create_link()
{ 
  mServer->lock();
  mServer->create_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "LINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

void Command::remove_link()
{ 
  mServer->lock();
  mServer->remove_link(mFrom, mFromPort, mToPort, mTo);
  mServer->unlock();

#ifdef DEBUG_PARSER
  std::cout << "UNLINK " << mFrom << "." << mFromPort << "=>" << mToPort << "." << mTo << std::endl;
#endif
}

// FIXME: execute_method should run in server space with concurrency locks.
void Command::execute_method()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mVariable)) {
    node->execute_method(mMethod, mParameters);
  } else {
    *mOutput << "Unknown node '" << mVariable << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_class_method()
{
  Class * klass;
  mServer->lock();
  if (Class::get(&klass, mClass)) {
    klass->execute_method(mMethod, mParameters, mOutput);
  } else {
    *mOutput << "Unknown class '" << mClass << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::execute_command()
{
  Node * node;
  mServer->lock();
  if (mServer->get_instance(&node, mMethod)) {
    // inspect
    *mOutput << node->inspect() << std::endl;
    
  } else if (mMethod == "quit" || mMethod == "q") {
    mServer->quit();
    mQuit = true;
  } else if (mMethod == "set_lib_path") {
    std::string path;
    if (!mParameters.get(&path)) {
      *mOutput << "Could not set library path (no parameter).\n";
      return;
    }
    Class::set_lib_path(path);
    *mOutput << "Library path set to '" << path << "'.\n";
  } else {
    *mOutput << "Unknown command '" << mMethod << "'" << std::endl;
  }
  mServer->unlock();
}

void Command::clear() 
{
  mTokenIndex = 0;
  mAction     = NO_ACTION;
  mVariable   = "";
  mClass      = "";
  mParameters.clear();
  mFromPort = 1;
  mToPort   = 1;
}
