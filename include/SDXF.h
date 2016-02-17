/* doc----------------------------------------------------------------
:Project.   Structured Data eXchange Format (SDXF)
:Program.   SDXF.h
:Title.     Basic definitions for SDXF (Structured Data eXchange Format)
:PrType.    Header File
:Version.   4
:Author.    Max Wildgrube
:Copyright. Max Wildgrube
:Language.  C (ANSI)
----------------------------------------------------------------------*/

#ifndef  SDXF_h
#define  SDXF_h
#include "sysdef.h"
#include "UTF8.h"

/* defaults:                      */

// #define SUPPORT_NATURAL

#ifdef AES_IMPLEMENTED            /* for internal use only */ 
  #define  EN_DEFAULT_ROUTINE SDXE_AES
#else
  #define  EN_DEFAULT_ROUTINE SDXE_RC4
#endif

#define TABLES_ENV  "SDXTRTAB"
#define TABLES_FILE "SDXTRANS.TAB"

/*END-defaults */

/* special definitions : */
/* timestamp:  */
#define    SDX_ID_Timestamp  65530  

typedef struct 
{
  Byte  year, month, day, hour, minute, seconds; // year: since 1900

} SDX_TTimestamp; 
/* END-special definitions : */

 

#define  SDX_MAGIC        32032
#define  SDX_CMPTYPE_NONE (Byte)0x00
#define  SDX_CMPTYPE_RL1  (Byte)0x01
#define  SDX_CMPTYPE_DEF  (Byte)0x02

#define  SETDT(f,d)       (f) &= 0x1F; (f) |= (d)<<5
#define  ISIN(s,b)        (Byte)(((s) & (1<<(b))) != 0)
#define  EXCL(s,b)        (s) &= ~(1<<(b))
#define  INCL(s,b)        (s) |= (1<<(b))


#define  SDX_IDlen           2
#define  SDX_HEADERLENGTH    (SDX_IDlen+4)
#define  SDX_blankID         0
#define  SDX_MAXKEYLEN       40
#define  SDX_ERRSTRLEN       100


enum /* SDXF Chunk flags */
{
   SDX_FLAGS               = 0x1F
,  SDX_FLAG_reserved1      = 0   /* 0x01 */
,  SDX_FLAG_array          = 1   /* 0x02 */
,  SDX_FLAG_short          = 2   /* 0x04 */
,  SDX_FLAG_encrypted      = 3   /* 0x08 */
,  SDX_FLAG_compressed     = 4   /* 0x10 */
};
 
typedef enum /* SDXF data types */
{
   SDX_DT_types           = 0xE0
,  SDX_DT_inconsistent    = 0    /* 0x00 */
,  SDX_DT_structured      = 1    /* 0x20 */
,  SDX_DT_binary          = 2    /* 0x40 */
,  SDX_DT_numeric         = 3    /* 0x60 */
,  SDX_DT_char            = 4    /* 0x80 */
,  SDX_DT_float           = 5    /* 0xA0 */ /* IEEE 754 */
,  SDX_DT_UTF8            = 6    /* 0xC0 */
,  SDX_DT_implemented     = 6    /* highest known DT number */

,  SDX_OPT_OLD            = 1
,  SDX_OPT_NEW            = 2
,  SDX_OPT_CLEAR          = 3    /* used for class SDXF dummy constructor */
,  SDX_OPT_STEP_BY_STEP   = -1
}  SDX_DataType;

enum      /* return codes */
{
   SDX_RC_ok               = 0
,  SDX_RC_warning          = 1
,  SDX_RC_failed           = 1
,  SDX_RC_illegalOperation = 2
,  SDX_RC_dataError        = 3
,  SDX_RC_parameterError   = 4
,  SDX_RC_programError     = 5
,  SDX_RC_noMemory         = 6
};

enum      /* extended return codes */ 
{
   SDX_EC_ok              =  0
,  SDX_EC_end_of_data     =  1
,  SDX_EC_notFound        =  2
,  SDX_EC_dataCutted      =  3
,  SDX_EC_overflow        =  4
,  SDX_EC_initError       =  5
,  SDX_EC_comprerr        =  6
,  SDX_EC_forbidden       =  7
,  SDX_EC_unknown         =  8
,  SDX_EC_levelOvflw      =  9
,  SDX_EC_paramMissing    = 10
,  SDX_EC_magicError      = 11
,  SDX_EC_not_consistent  = 12
,  SDX_EC_wrongDataType   = 13
,  SDX_EC_noMemory        = 14
,  SDX_EC_malformed       = 15    /* malformed UTF-8 sequence              */ 
,  SDX_EC_crypterror      = 16    /* en/de-cryption error (length?)        */ 
,  SDX_EC_wrongLength     = 17    /* dataLength wrong (0 or != 2,4 for num */ 
,  SDX_EC_FTP_basic       = 17
,  SDX_EC_FTP_inexact     = 18    /* +1 codes for floating point           */ 
,  SDX_EC_FTP_invalid     = 19    /* +2   conversion  IEEE to /390 mode    */  
,  SDX_EC_FTP_underflow   = 20    /* +3   and v.v. (mainframe only)        */  
,  SDX_EC_FTP_overflow    = 21    /* +4                                    */ 
,  SDX_EC_unknown_function= 22
,  SDX_EC_cont_too_small  = 23
,  SDX_EC_not_supported   = 24
,  SDX_EC_data_missing    = 25    /* data == NULL or maxLength <= 0        */
,  SDX_EC_not_structured  = 26
,  SDX_EC_highestNumber   = 29    /* max. errcodes reserved for SDXF       */ 
,  SDX_EC_error           = 99
};



/*   Types and Structures:  */ 

typedef unsigned short  ChunkID;

typedef struct SDX_Chunk
{
  ChunkID    chunkID;
  Byte       flags;
  Byte       length [3];
  Byte       data; /*or:  [100]; // for debug */ 
} SDX_Chunk;

/**********for debug: ********************/
typedef struct TPrivate                
{                              
    SDX_Chunk *newChunk;       
    long       workbufflen;    
    Byte      *workbuffer;     

    ChunkID    oldChunkID;     
    Byte       iflags;         
                               
    struct     T_stack         
    {                          
      SDX_Chunk *chunk;        
      long       length, rest; 
    } stack [1];                

} TPrivate;

#define SDX_PRIVATE TPrivate    
/**********end- debug: ********************/

typedef struct SDX_obj
{
  ChunkID    magic;         /* = 32032: check template                */ 
  ChunkID    chunkID;       /* Name (ID) of Chunk                     */ 
  Byte      *container;     /* Pointer to the whole Chunk             */
  long       bufferSize;    /* size of container                      */ 
  SDX_Chunk *currChunk;     /* Pointer to actual Chunk                */ 
  long       dataLength;    /* Length of data in Chunk                */ 
  long       maxLength;     /* Maximal length of Chunk at COPY        */ 
  long       remainingSize; /* rem. size in cont. aft. CREATE         */ 
  long       value;         /* for data type numeric / init option    */ 
  double     fvalue;        /* for data type float                    */ 
  char      *function;      /* name of the executed SDX function      */ 
  void      *data;          /* Pointer to Data                        */ 
  Byte      *cryptkey;      /* Pointer to Crypt Key                   */ 
  short      count;         /* (max.) number of elements in an array  */ 
  short      arr_index;      
  short      dataType;      /* Chunk data type | init open type       */ 
  short      ec;            /* Extended return-code                   */ 
  short      rc;            /* return-code                            */ 
  short      level;         /* Level of hierarchy                     */ 
  char       filler;        /* filler char for SDX_extract            */ 
  Byte       encrypt;       /* Indication if data to encrypt (0 / 1)  */ 
  Byte       compression;   /* Compression Method                     */ 
                            /*   (00=none, 01=RL1, 02=DEFLATE)        */ 
                               /* private section: */  
  #ifndef SDX_PRIVATE                                                                  /*#32(*/      
    #define SDX_PRIVATE void
  #endif 
  SDX_PRIVATE *private_stuff;                                                          /*#32)*/

} SDX_obj, *SDX_handle;

typedef enum Tcrypt_mode 
  { CM_ENCRYPT = 1
  , CM_DECRYPT
  , CM_GET_BLOCKSIZE
  , CM_GET_VERSION
  , CM_SET_IV
  , CM_SHIFTnMODIFY_IV                                                                  /*#13*/ 
  , CM_SET_BLOCKING
  }  
  Tcrypt_mode;

typedef 
  long TEncryptProc   
         ( Tcrypt_mode  mode
         , Byte             *buffer
         , long              len
         , char             *passw
         );

typedef 
  bool TGetTablesProc (Byte **toNet, Byte **toHost);

typedef struct     /* Options Table */ 
{
  Byte           *toHost;        /* Trans tab  net -> host                  */ 
  Byte           *toNet;         /* Trans tab  host -> net                  */ 
  int             maxLevel;      /* highest possible level                  */ 
  int             translation;   /* char conversion net <-> host            */ 
                                 /* is in effect=1 or not=0                 */    
  TEncryptProc   *encryptProc;
  TGetTablesProc *getTablesProc;
  UTF8_TCvtProc  *cvtUTF8Proc;

  /* beyond of RFC3072:  */
  char   *version;         /* SDXF version string                           */
  char   *zlib_version;    /* ZLIB version string                           */
  int     adaptFTPto390;   /* floating point conversion net <-> host        */
                           /* is in effect=1 or not=0 (mainframe only)      */
  int     encryptBlocking; /* encrypted chunk is expanded to Blocksize (=1) */
                           /* or is length-preserving (=0) (for blocking    */
                           /* encryption algorithms only, like AES)         */
  int     max_array_dim;   /* used for calc. workbuffer for compression     */
                           /* in case of creating arrays step by step       */
}  SDX_TOptions;

/* The param struct for the Single Entry Interface (SEI): */
#define  SDX_NAMLEN   32
#define  SDXE_PASSLEN 16

typedef struct 
{ 
  char        magic [4];    /*  'SDXF' A    4   000 magic for test          */ 
  SDX_handle  handle;       /*  F      B    4   004 internal                */ 
  long        value;        /*  F      I    4   008 binary value            */ 
  double      fvalue;       /*  D      F    8   012 floating binary value   */          /*#33*/
  long        length;       /*  F      I    4   020 data length             */          
  long        bufferSize;   /*  F      I    4   024 size of SDXF container  */          
  short       chunkID;      /*  H      I    2   028                         */          
  short       dataType;     /*  H      I    2   030 SDXF data type          */          
  short       count;        /*  H      I    2   030 array count             */          
  short       function;     /*  H      I    2   032 one of f_               */          
  short       rc;           /*  H      I    2   034 return code             */          
  short       ec;           /*  H      I    2   036 extended rc             */          
  short       level;        /*  H      I    2   038 SDXF struct level       */          /*#33*/
  short       do_encrypt;   /*  H      I    2   040 = 0 or 1                */          /*#33*/
  short       compr;        /*  H      I    2   042 compression method      */          /*#33*/
  char        option;       /*  C1     A    1   044 option (SDX_open)       */          /*#33*/ 
  char name  [SDX_NAMLEN];  /*  CL32   A   32   045 actual function name    */          
  char passw [SDXE_PASSLEN];/*  CL16   A   16   077 encryptkey (password)   */           
}  SDX_PARAM;               /*                  093 length of param struct  */          /*#33*/

enum /* functions for SDXF single entry interface */ 
{
    f_sdx_get_version = 0 /* out: NAME: version string               0  */ 
  , f_sdx_init_old        /* todo: in OPTION: old='O', new='N'       1  */ 
  , f_sdx_init_new        /* todo: !!! obsolete by init!             2  */ 
  , f_sdx_enter           /* out: CHUNK-ID                           3  */ 
  , f_sdx_leave           /* out: CHUNK-ID                           4  */ 
  , f_sdx_next            /* out: CHUNK-ID                           5  */ 
  , f_sdx_select          /* in: CHUNK-ID                            6  */ 
  , f_sdx_extract         /* in: MAX-LENGTH, out: DATA               7  */ 
  , f_sdx_create          /* in: LENGTH, CHUNK-ID, DATA,             8  */ 
                          /*     DATA-TYPE, DO-ENCRYPTION               */ 
  , f_sdx_append          /* in: DATA                                9  */ 
  , f_sdx_adapt_short     /* inout: VALUE                           10  */ 
  , f_sdx_adapt_long      /* inout: VALUE                           11  */ 
  , f_sdx_get_buffer      /* in: LENGTH (allocates memory)          12  */ 
  , f_sdx_read            /*                                        13  */ 
  , f_sdx_write           /*                                        14  */ 
  , f_sdx_file_open       /* in: NAME, OPTION ('R', 'W')            15  */ 
  , f_sdx_file_close      /*                                        16  */ 
  , f_sdx_set_key         /* set encryption key                     17  */                //#34
  , f_sdx_get_err_text    /* out: DATA: text interpr. rc, ec        18  */ 
};


#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/ 


#ifdef _SYS_MAINFRAME
  void SDXF (SDX_PARAM *par, Byte *data);

#else
  #ifdef  SUPPORT_NATURAL
    #include "natuser.h"
    DllExport NATFCT SDXFN NATARGDCL(nparm, parmptr, parmdec);
  #endif
 
  DllExport long CC sdxfB (SDX_PARAM *param, Byte *data);
  DllExport long    sdxfC (SDX_PARAM *param, Byte *data);
  DllExport void    sdxfS 
                    (
                     SDX_handle *handle,       /*  1 F   4  internal                */ 
                     long       *value,        /*  2 F   4  binary value            */ 
                     double     *fvalue,       /*  3 D   8  floating binary value   */  /*#33*/
                     long       *length,       /*  4 F   4  data length             */ 
                     long       *bufferSize,   /*  5 F   4  size of SDXF container  */ 
                     short      *chunkID,      /*  6 H   2                          */ 
                     short      *dataType,     /*  7 H   2  SDXF data type          */ 
                     short      *count,        /*  8 H   2  array count             */ 
                     short      *function,     /*  9 H   2  one of f_               */ 
                     short      *rc,           /* 10 H   2  return code             */ 
                     short      *ec,           /* 11 H   2  extended rc             */ 
                     short      *level,        /* 12 H   2  SDXF struct level       */  /*#33*/
                     short      *do_encrypt,   /* 13 H   2  = 0 or 1                */  /*#33*/
                     short      *compr,        /* 14 H   2  compression method      */  /*#33*/
                     char       *option,       /* 15 C   1  option (SDX_open)       */  /*#33*/ 
                     char       *name,         /* 16 C  32  actual function name    */ 
                     //   char       *passw,        /* 17 C  16  encryptkey (password)    #34: obsolete   */ 
                     char       *data          /* 18 C 200  char data               */  
                    );
#endif /* platform */


#ifdef __cplusplus
} // extern-C 
#endif

/* definitions for encryption: */
#define  SDXE_BLKSIZE   16

/*     Prototypes     */ 

#ifdef _SYS_MAINFRAME
#define  SDX_init            SXinit
#define  SDX_enter           SXenter
#define  SDX_leave           SXleave
#define  SDX_next            SXnext
#define  SDX_select          SXselect
#define  SDX_extract         SXextrct
#define  SDX_append          SXappend
#define  SDX_create          SXcreate
#define  SDX_getOptions      SXgetOpt
#define  SDX_update          SXupdate
#define  SDX_check           SXcheck
#define  SDX_errtext         SXerrtxt
#define  SDX_errtextB        SXerrtxB
#define  SDX_setKey          SXsetkey                                                     //#34

#define  SDXE_keyhash        SXkyhash 
#define  SDXE_RC4            SXRC4
#define  SDXE_AES            SXAES
#endif

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/ 


DllExport  void  SDX_init     (SDX_obj *);
DllExport  void  SDX_enter    (SDX_obj *);
DllExport  void  SDX_leave    (SDX_obj *);
DllExport  void  SDX_next     (SDX_obj *);
DllExport  void  SDX_select   (SDX_obj *);
DllExport  void  SDX_extract  (SDX_obj *);
DllExport  void  SDX_append   (SDX_obj *);
DllExport  void  SDX_create   (SDX_obj *);
DllExport  void  SDX_update   (SDX_obj *);
DllExport  void  SDX_check    (SDX_obj *);
DllExport  void  SDX_setKey   (SDX_obj *);                                                //#34
DllExport  char *SDX_errtext  (SDX_obj *);
DllExport  char *SDX_errtextB (short rc, short ec, char *buffer);

DllExport  SDX_TOptions *SDX_getOptions ();

/* definitions for encryption: */

#define SDXE_SHA_HASHVALUE_LEN  (sizeof(long)*5)  /*  = 20  */ 
        
short  SDXE_hash_length ();

DllExport Byte  *SDXE_keyhash 
  ( Byte          *hash       /* out : the hash value [20]  */ 
  , long           hlen       /* in  : length of output     */  
  , char          *str1       /* in  : the key              */ 
  , long           len1       /* in  : length of key        */ 
  , char          *str2       /* in  : the text             */ 
  , long           len2       /* in  : length of text       */ 
  );
  /* returns pointer to the hash value (max 20 bytes) */ 


DllExport long SDXE_RC4 
  ( Tcrypt_mode   mode  /* encrypt, decrypt or BLOCKSIZE           */ 
  , Byte         *buffer    /* content to encrypt                  */ 
  , long          len       /* length of buffer                    */ 
  , char         *passw     /* encryption key                      */ 
  );

/************************************************************
results:  success
*************************************************************/

DllExport long SDXE_AES 
  ( Tcrypt_mode   mode  /* encrypt, decrypt, SET-IV, SHIFT-&-MODIFY-IV */ 
                        /* or BLOCKSIZE */ 
  , Byte         *buffer    /* content to encrypt / encIV          */ 
  , long          len       /* length of buffer                    */ 
  , char         *passw     /* encryption key     / decIV          */ 
  );

/************************************************************
results: length of output / Blocksize
*************************************************************/


#ifdef __cplusplus
}//end-extern-C 

/////// C++ definitions //////////////////////

#include <stdio.h>

const bool isUTF8 = true;

class DllExport SDXC_CHUNK
{
  public:
  
  SDXC_CHUNK (void);
  SDXC_CHUNK (ChunkID);
  SDXC_CHUNK (ChunkID, long value);
  SDXC_CHUNK (ChunkID, double fvalue);
  SDXC_CHUNK (ChunkID, char *data);
  SDXC_CHUNK (ChunkID, char *data, long len);
  SDXC_CHUNK (ChunkID, char *data, long len, bool is_utf8);
  SDXC_CHUNK (ChunkID, Byte *data, long len);
  
  long     value, length;
  char    *data;
  ChunkID  id;
  Byte     dt; 
};

class DllExport SDXF
{
  public:
  SDXF  ();                            // dummy
  SDXF  (Byte *cont);                  // old container
  SDXF  (Byte *cont, long size);       // new container
  SDXF  (long size);                   // new container
  ~SDXF ();

  // conventional interface:

  void init  (void);                   // old container
  void init  (Byte *cont);             // old container
  void init  (Byte *cont, long size);  // new container
  void init  (long size);              // new container

  void enter (void);
  void leave (void);
  void next  (void);

  long  extract (void *data, long  length);         // extract char, binary, UTF8 
  long  extract (void);                             // extract numeric
  void  extract (long   *);                         // extract numeric
  void  extract (double *);                         // extract float
  short extract (void *data, short length, short max_count);  // array

  void create  (ChunkID);                           // structured
  void create  (ChunkID, long);                     // integer numeric
  void create  (ChunkID, double);                   // float numeric
  void create  (ChunkID, Byte *data, long length);  // binary
  void create  (ChunkID, char *data);               // character
  void create  (ChunkID, char *data, bool is_utf8); // 3rd parm=true: unicode character as UTF8 
  void create  (ChunkID, void *data, short count, short length, SDX_DataType); // array

  void setKey  (char *data);                                                              //#34

  void write   (FILE *);
  void read    (FILE *);

  void set_compression (Byte  compression_method);
  void set_encryption  (Byte *encryption_key);

  // advanced interface:
  
  void operator () (void);                   // alias for 'init NEW'
  void operator () (Byte *cont);             // old container
  void operator () (Byte *cont, long size);  // new container
  void operator () (long size);              // new container
  void operator ~  (void);    // alias for 'enter'
  void operator !  (void);    // alias for 'leave'
  bool operator ++ (int);     // alias for 'next'    (returns success)

  void operator >> (FILE *);  // alias for 'write'  
  void operator << (FILE *);  // alias for 'read'  

  // common:

  short    ec;                // extended return code  
  short    rc;                // raw return code  
  bool     ok;                // rc == SDX_RC_ok 
  long     length;            // data length

  ChunkID  id;                // chunk id
  short    dataType;

  int      line;              // error source program line 
  char    *program;           // error source program

  //  friend:

  bool     mycont;
  Byte    *cont;
  SDX_obj  sdx;

  private:
 
  //void operator -- (int); for what?
};

DllExport void operator >> (SDXF&, SDXC_CHUNK&);    // alias for 'extract'  
DllExport void operator << (SDXF&, SDXC_CHUNK&);    // alias for 'create'  

#endif /*__cplusplus*/

#endif /*SDXF_h*/
/*****************  end of source  ****************/
