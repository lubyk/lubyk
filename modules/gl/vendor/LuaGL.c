/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  www.luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the OpenGL
*               binding for Lua 5.0.
*-------------------------------------------------
*  Last Update: 14/07/2004
*  Version: v1.01
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#include "LuaGL.h"

#define ENUM_ERROR (GLenum)-2

typedef struct gl_str_value {
  const char *str;
  GLenum value;
} gl_str_value;

static const gl_str_value gl_str[] = {
   { "VERSION_1_1"                     , 1 },
   { "ACCUM"                           , 0x0100 },
   { "LOAD"                            , 0x0101 },
   { "RETURN"                          , 0x0102 },
   { "MULT"                            , 0x0103 },
   { "ADD"                             , 0x0104 },
   { "NEVER"                           , 0x0200 },
   { "LESS"                            , 0x0201 },
   { "EQUAL"                           , 0x0202 },
   { "LEQUAL"                          , 0x0203 },
   { "GREATER"                         , 0x0204 },
   { "NOTEQUAL"                        , 0x0205 },
   { "GEQUAL"                          , 0x0206 },
   { "ALWAYS"                          , 0x0207 },
   { "POINTS"                          , 0x0000 },
   { "LINES"                           , 0x0001 },
   { "LINE_LOOP"                       , 0x0002 },
   { "LINE_STRIP"                      , 0x0003 },
   { "TRIANGLES"                       , 0x0004 },
   { "TRIANGLE_STRIP"                  , 0x0005 },
   { "TRIANGLE_FAN"                    , 0x0006 },
   { "QUADS"                           , 0x0007 },
   { "QUAD_STRIP"                      , 0x0008 },
   { "POLYGON"                         , 0x0009 },
   { "ZERO"                            , 0 },
   { "ONE"                             , 1 },
   { "SRC_COLOR"                       , 0x0300 },
   { "ONE_MINUS_SRC_COLOR"             , 0x0301 },
   { "SRC_ALPHA"                       , 0x0302 },
   { "ONE_MINUS_SRC_ALPHA"             , 0x0303 },
   { "DST_ALPHA"                       , 0x0304 },
   { "ONE_MINUS_DST_ALPHA"             , 0x0305 },
   { "DST_COLOR"                       , 0x0306 },
   { "ONE_MINUS_DST_COLOR"             , 0x0307 },
   { "SRC_ALPHA_SATURATE"              , 0x0308 },
   { "TRUE"                            , 1 },
   { "FALSE"                           , 0 },
   { "CLIP_PLANE0"                     , 0x3000 },
   { "CLIP_PLANE1"                     , 0x3001 },
   { "CLIP_PLANE2"                     , 0x3002 },
   { "CLIP_PLANE3"                     , 0x3003 },
   { "CLIP_PLANE4"                     , 0x3004 },
   { "CLIP_PLANE5"                     , 0x3005 },
   { "BYTE"                            , 0x1400 },
   { "UNSIGNED_BYTE"                   , 0x1401 },
   { "SHORT"                           , 0x1402 },
   { "UNSIGNED_SHORT"                  , 0x1403 },
   { "INT"                             , 0x1404 },
   { "UNSIGNED_INT"                    , 0x1405 },
   { "FLOAT"                           , 0x1406 },
   { "2_BYTES"                         , 0x1407 },
   { "3_BYTES"                         , 0x1408 },
   { "4_BYTES"                         , 0x1409 },
   { "DOUBLE"                          , 0x140A },
   { "NONE"                            , 0 },
   { "FRONT_LEFT"                      , 0x0400 },
   { "FRONT_RIGHT"                     , 0x0401 },
   { "BACK_LEFT"                       , 0x0402 },
   { "BACK_RIGHT"                      , 0x0403 },
   { "FRONT"                           , 0x0404 },
   { "BACK"                            , 0x0405 },
   { "LEFT"                            , 0x0406 },
   { "RIGHT"                           , 0x0407 },
   { "FRONT_AND_BACK"                  , 0x0408 },
   { "AUX0"                            , 0x0409 },
   { "AUX1"                            , 0x040A },
   { "AUX2"                            , 0x040B },
   { "AUX3"                            , 0x040C },
   { "NO_ERROR"                        , 0 },
   { "INVALID_ENUM"                    , 0x0500 },
   { "INVALID_VALUE"                   , 0x0501 },
   { "INVALID_OPERATION"               , 0x0502 },
   { "STACK_OVERFLOW"                  , 0x0503 },
   { "STACK_UNDERFLOW"                 , 0x0504 },
   { "OUT_OF_MEMORY"                   , 0x0505 },
   { "2D"                              , 0x0600 },
   { "3D"                              , 0x0601 },
   { "3D_COLOR"                        , 0x0602 },
   { "3D_COLOR_TEXTURE"                , 0x0603 },
   { "4D_COLOR_TEXTURE"                , 0x0604 },
   { "PASS_THROUGH_TOKEN"              , 0x0700 },
   { "POINT_TOKEN"                     , 0x0701 },
   { "LINE_TOKEN"                      , 0x0702 },
   { "POLYGON_TOKEN"                   , 0x0703 },
   { "BITMAP_TOKEN"                    , 0x0704 },
   { "DRAW_PIXEL_TOKEN"                , 0x0705 },
   { "COPY_PIXEL_TOKEN"                , 0x0706 },
   { "LINE_RESET_TOKEN"                , 0x0707 },
   { "EXP"                             , 0x0800 },
   { "EXP2"                            , 0x0801 },
   { "CW"                              , 0x0900 },
   { "CCW"                             , 0x0901 },
   { "COEFF"                           , 0x0A00 },
   { "ORDER"                           , 0x0A01 },
   { "DOMAIN"                          , 0x0A02 },
   { "CURRENT_COLOR"                   , 0x0B00 },
   { "CURRENT_INDEX"                   , 0x0B01 },
   { "CURRENT_NORMAL"                  , 0x0B02 },
   { "CURRENT_TEXTURE_COORDS"          , 0x0B03 },
   { "CURRENT_RASTER_COLOR"            , 0x0B04 },
   { "CURRENT_RASTER_INDEX"            , 0x0B05 },
   { "CURRENT_RASTER_TEXTURE_COORDS"   , 0x0B06 },
   { "CURRENT_RASTER_POSITION"         , 0x0B07 },
   { "CURRENT_RASTER_POSITION_VALID"   , 0x0B08 },
   { "CURRENT_RASTER_DISTANCE"         , 0x0B09 },
   { "POINT_SMOOTH"                    , 0x0B10 },
   { "POINT_SIZE"                      , 0x0B11 },
   { "POINT_SIZE_RANGE"                , 0x0B12 },
   { "POINT_SIZE_GRANULARITY"          , 0x0B13 },
   { "LINE_SMOOTH"                     , 0x0B20 },
   { "LINE_WIDTH"                      , 0x0B21 },
   { "LINE_WIDTH_RANGE"                , 0x0B22 },
   { "LINE_WIDTH_GRANULARITY"          , 0x0B23 },
   { "LINE_STIPPLE"                    , 0x0B24 },
   { "LINE_STIPPLE_PATTERN"            , 0x0B25 },
   { "LINE_STIPPLE_REPEAT"             , 0x0B26 },
   { "LIST_MODE"                       , 0x0B30 },
   { "MAX_LIST_NESTING"                , 0x0B31 },
   { "LIST_BASE"                       , 0x0B32 },
   { "LIST_INDEX"                      , 0x0B33 },
   { "POLYGON_MODE"                    , 0x0B40 },
   { "POLYGON_SMOOTH"                  , 0x0B41 },
   { "POLYGON_STIPPLE"                 , 0x0B42 },
   { "EDGE_FLAG"                       , 0x0B43 },
   { "CULL_FACE"                       , 0x0B44 },
   { "CULL_FACE_MODE"                  , 0x0B45 },
   { "FRONT_FACE"                      , 0x0B46 },
   { "LIGHTING"                        , 0x0B50 },
   { "LIGHT_MODEL_LOCAL_VIEWER"        , 0x0B51 },
   { "LIGHT_MODEL_TWO_SIDE"            , 0x0B52 },
   { "LIGHT_MODEL_AMBIENT"             , 0x0B53 },
   { "SHADE_MODEL"                     , 0x0B54 },
   { "COLOR_MATERIAL_FACE"             , 0x0B55 },
   { "COLOR_MATERIAL_PARAMETER"        , 0x0B56 },
   { "COLOR_MATERIAL"                  , 0x0B57 },
   { "FOG"                             , 0x0B60 },
   { "FOG_INDEX"                       , 0x0B61 },
   { "FOG_DENSITY"                     , 0x0B62 },
   { "FOG_START"                       , 0x0B63 },
   { "FOG_END"                         , 0x0B64 },
   { "FOG_MODE"                        , 0x0B65 },
   { "FOG_COLOR"                       , 0x0B66 },
   { "DEPTH_RANGE"                     , 0x0B70 },
   { "DEPTH_TEST"                      , 0x0B71 },
   { "DEPTH_WRITEMASK"                 , 0x0B72 },
   { "DEPTH_CLEAR_VALUE"               , 0x0B73 },
   { "DEPTH_FUNC"                      , 0x0B74 },
   { "ACCUM_CLEAR_VALUE"               , 0x0B80 },
   { "STENCIL_TEST"                    , 0x0B90 },
   { "STENCIL_CLEAR_VALUE"             , 0x0B91 },
   { "STENCIL_FUNC"                    , 0x0B92 },
   { "STENCIL_VALUE_MASK"              , 0x0B93 },
   { "STENCIL_FAIL"                    , 0x0B94 },
   { "STENCIL_PASS_DEPTH_FAIL"         , 0x0B95 },
   { "STENCIL_PASS_DEPTH_PASS"         , 0x0B96 },
   { "STENCIL_REF"                     , 0x0B97 },
   { "STENCIL_WRITEMASK"               , 0x0B98 },
   { "MATRIX_MODE"                     , 0x0BA0 },
   { "NORMALIZE"                       , 0x0BA1 },
   { "VIEWPORT"                        , 0x0BA2 },
   { "MODELVIEW_STACK_DEPTH"           , 0x0BA3 },
   { "PROJECTION_STACK_DEPTH"          , 0x0BA4 },
   { "TEXTURE_STACK_DEPTH"             , 0x0BA5 },
   { "MODELVIEW_MATRIX"                , 0x0BA6 },
   { "PROJECTION_MATRIX"               , 0x0BA7 },
   { "TEXTURE_MATRIX"                  , 0x0BA8 },
   { "ATTRIB_STACK_DEPTH"              , 0x0BB0 },
   { "CLIENT_ATTRIB_STACK_DEPTH"       , 0x0BB1 },
   { "ALPHA_TEST"                      , 0x0BC0 },
   { "ALPHA_TEST_FUNC"                 , 0x0BC1 },
   { "ALPHA_TEST_REF"                  , 0x0BC2 },
   { "DITHER"                          , 0x0BD0 },
   { "BLEND_DST"                       , 0x0BE0 },
   { "BLEND_SRC"                       , 0x0BE1 },
   { "BLEND"                           , 0x0BE2 },
   { "LOGIC_OP_MODE"                   , 0x0BF0 },
   { "LOGIC_OP"                        , 0x0BF1 },
   { "INDEX_LOGIC_OP"                  , 0x0BF1 },
   { "COLOR_LOGIC_OP"                  , 0x0BF2 },
   { "AUX_BUFFERS"                     , 0x0C00 },
   { "DRAW_BUFFER"                     , 0x0C01 },
   { "READ_BUFFER"                     , 0x0C02 },
   { "SCISSOR_BOX"                     , 0x0C10 },
   { "SCISSOR_TEST"                    , 0x0C11 },
   { "INDEX_CLEAR_VALUE"               , 0x0C20 },
   { "INDEX_WRITEMASK"                 , 0x0C21 },
   { "COLOR_CLEAR_VALUE"               , 0x0C22 },
   { "COLOR_WRITEMASK"                 , 0x0C23 },
   { "INDEX_MODE"                      , 0x0C30 },
   { "RGBA_MODE"                       , 0x0C31 },
   { "DOUBLEBUFFER"                    , 0x0C32 },
   { "STEREO"                          , 0x0C33 },
   { "RENDER_MODE"                     , 0x0C40 },
   { "PERSPECTIVE_CORRECTION_HINT"     , 0x0C50 },
   { "POINT_SMOOTH_HINT"               , 0x0C51 },
   { "LINE_SMOOTH_HINT"                , 0x0C52 },
   { "POLYGON_SMOOTH_HINT"             , 0x0C53 },
   { "FOG_HINT"                        , 0x0C54 },
   { "TEXTURE_GEN_S"                   , 0x0C60 },
   { "TEXTURE_GEN_T"                   , 0x0C61 },
   { "TEXTURE_GEN_R"                   , 0x0C62 },
   { "TEXTURE_GEN_Q"                   , 0x0C63 },
   { "PIXEL_MAP_I_TO_I"                , 0x0C70 },
   { "PIXEL_MAP_S_TO_S"                , 0x0C71 },
   { "PIXEL_MAP_I_TO_R"                , 0x0C72 },
   { "PIXEL_MAP_I_TO_G"                , 0x0C73 },
   { "PIXEL_MAP_I_TO_B"                , 0x0C74 },
   { "PIXEL_MAP_I_TO_A"                , 0x0C75 },
   { "PIXEL_MAP_R_TO_R"                , 0x0C76 },
   { "PIXEL_MAP_G_TO_G"                , 0x0C77 },
   { "PIXEL_MAP_B_TO_B"                , 0x0C78 },
   { "PIXEL_MAP_A_TO_A"                , 0x0C79 },
   { "PIXEL_MAP_I_TO_I_SIZE"           , 0x0CB0 },
   { "PIXEL_MAP_S_TO_S_SIZE"           , 0x0CB1 },
   { "PIXEL_MAP_I_TO_R_SIZE"           , 0x0CB2 },
   { "PIXEL_MAP_I_TO_G_SIZE"           , 0x0CB3 },
   { "PIXEL_MAP_I_TO_B_SIZE"           , 0x0CB4 },
   { "PIXEL_MAP_I_TO_A_SIZE"           , 0x0CB5 },
   { "PIXEL_MAP_R_TO_R_SIZE"           , 0x0CB6 },
   { "PIXEL_MAP_G_TO_G_SIZE"           , 0x0CB7 },
   { "PIXEL_MAP_B_TO_B_SIZE"           , 0x0CB8 },
   { "PIXEL_MAP_A_TO_A_SIZE"           , 0x0CB9 },
   { "UNPACK_SWAP_BYTES"               , 0x0CF0 },
   { "UNPACK_LSB_FIRST"                , 0x0CF1 },
   { "UNPACK_ROW_LENGTH"               , 0x0CF2 },
   { "UNPACK_SKIP_ROWS"                , 0x0CF3 },
   { "UNPACK_SKIP_PIXELS"              , 0x0CF4 },
   { "UNPACK_ALIGNMENT"                , 0x0CF5 },
   { "PACK_SWAP_BYTES"                 , 0x0D00 },
   { "PACK_LSB_FIRST"                  , 0x0D01 },
   { "PACK_ROW_LENGTH"                 , 0x0D02 },
   { "PACK_SKIP_ROWS"                  , 0x0D03 },
   { "PACK_SKIP_PIXELS"                , 0x0D04 },
   { "PACK_ALIGNMENT"                  , 0x0D05 },
   { "MAP_COLOR"                       , 0x0D10 },
   { "MAP_STENCIL"                     , 0x0D11 },
   { "INDEX_SHIFT"                     , 0x0D12 },
   { "INDEX_OFFSET"                    , 0x0D13 },
   { "RED_SCALE"                       , 0x0D14 },
   { "RED_BIAS"                        , 0x0D15 },
   { "ZOOM_X"                          , 0x0D16 },
   { "ZOOM_Y"                          , 0x0D17 },
   { "GREEN_SCALE"                     , 0x0D18 },
   { "GREEN_BIAS"                      , 0x0D19 },
   { "BLUE_SCALE"                      , 0x0D1A },
   { "BLUE_BIAS"                       , 0x0D1B },
   { "ALPHA_SCALE"                     , 0x0D1C },
   { "ALPHA_BIAS"                      , 0x0D1D },
   { "DEPTH_SCALE"                     , 0x0D1E },
   { "DEPTH_BIAS"                      , 0x0D1F },
   { "MAX_EVAL_ORDER"                  , 0x0D30 },
   { "MAX_LIGHTS"                      , 0x0D31 },
   { "MAX_CLIP_PLANES"                 , 0x0D32 },
   { "MAX_TEXTURE_SIZE"                , 0x0D33 },
   { "MAX_PIXEL_MAP_TABLE"             , 0x0D34 },
   { "MAX_ATTRIB_STACK_DEPTH"          , 0x0D35 },
   { "MAX_MODELVIEW_STACK_DEPTH"       , 0x0D36 },
   { "MAX_NAME_STACK_DEPTH"            , 0x0D37 },
   { "MAX_PROJECTION_STACK_DEPTH"      , 0x0D38 },
   { "MAX_TEXTURE_STACK_DEPTH"         , 0x0D39 },
   { "MAX_VIEWPORT_DIMS"               , 0x0D3A },
   { "MAX_CLIENT_ATTRIB_STACK_DEPTH"   , 0x0D3B },
   { "SUBPIXEL_BITS"                   , 0x0D50 },
   { "INDEX_BITS"                      , 0x0D51 },
   { "RED_BITS"                        , 0x0D52 },
   { "GREEN_BITS"                      , 0x0D53 },
   { "BLUE_BITS"                       , 0x0D54 },
   { "ALPHA_BITS"                      , 0x0D55 },
   { "DEPTH_BITS"                      , 0x0D56 },
   { "STENCIL_BITS"                    , 0x0D57 },
   { "ACCUM_RED_BITS"                  , 0x0D58 },
   { "ACCUM_GREEN_BITS"                , 0x0D59 },
   { "ACCUM_BLUE_BITS"                 , 0x0D5A },
   { "ACCUM_ALPHA_BITS"                , 0x0D5B },
   { "NAME_STACK_DEPTH"                , 0x0D70 },
   { "AUTO_NORMAL"                     , 0x0D80 },
   { "MAP1_COLOR_4"                    , 0x0D90 },
   { "MAP1_INDEX"                      , 0x0D91 },
   { "MAP1_NORMAL"                     , 0x0D92 },
   { "MAP1_TEXTURE_COORD_1"            , 0x0D93 },
   { "MAP1_TEXTURE_COORD_2"            , 0x0D94 },
   { "MAP1_TEXTURE_COORD_3"            , 0x0D95 },
   { "MAP1_TEXTURE_COORD_4"            , 0x0D96 },
   { "MAP1_VERTEX_3"                   , 0x0D97 },
   { "MAP1_VERTEX_4"                   , 0x0D98 },
   { "MAP2_COLOR_4"                    , 0x0DB0 },
   { "MAP2_INDEX"                      , 0x0DB1 },
   { "MAP2_NORMAL"                     , 0x0DB2 },
   { "MAP2_TEXTURE_COORD_1"            , 0x0DB3 },
   { "MAP2_TEXTURE_COORD_2"            , 0x0DB4 },
   { "MAP2_TEXTURE_COORD_3"            , 0x0DB5 },
   { "MAP2_TEXTURE_COORD_4"            , 0x0DB6 },
   { "MAP2_VERTEX_3"                   , 0x0DB7 },
   { "MAP2_VERTEX_4"                   , 0x0DB8 },
   { "MAP1_GRID_DOMAIN"                , 0x0DD0 },
   { "MAP1_GRID_SEGMENTS"              , 0x0DD1 },
   { "MAP2_GRID_DOMAIN"                , 0x0DD2 },
   { "MAP2_GRID_SEGMENTS"              , 0x0DD3 },
   { "TEXTURE_1D"                      , 0x0DE0 },
   { "TEXTURE_2D"                      , 0x0DE1 },
   { "FEEDBACK_BUFFER_POINTER"         , 0x0DF0 },
   { "FEEDBACK_BUFFER_SIZE"            , 0x0DF1 },
   { "FEEDBACK_BUFFER_TYPE"            , 0x0DF2 },
   { "SELECTION_BUFFER_POINTER"        , 0x0DF3 },
   { "SELECTION_BUFFER_SIZE"           , 0x0DF4 },
   { "TEXTURE_WIDTH"                   , 0x1000 },
   { "TEXTURE_HEIGHT"                  , 0x1001 },
   { "TEXTURE_COMPONENTS"              , 0x1003 },
   { "TEXTURE_INTERNAL_FORMAT"         , 0x1003 },
   { "TEXTURE_BORDER_COLOR"            , 0x1004 },
   { "TEXTURE_BORDER"                  , 0x1005 },
   { "DONT_CARE"                       , 0x1100 },
   { "FASTEST"                         , 0x1101 },
   { "NICEST"                          , 0x1102 },
   { "LIGHT0"                          , 0x4000 },
   { "LIGHT1"                          , 0x4001 },
   { "LIGHT2"                          , 0x4002 },
   { "LIGHT3"                          , 0x4003 },
   { "LIGHT4"                          , 0x4004 },
   { "LIGHT5"                          , 0x4005 },
   { "LIGHT6"                          , 0x4006 },
   { "LIGHT7"                          , 0x4007 },
   { "AMBIENT"                         , 0x1200 },
   { "DIFFUSE"                         , 0x1201 },
   { "SPECULAR"                        , 0x1202 },
   { "POSITION"                        , 0x1203 },
   { "SPOT_DIRECTION"                  , 0x1204 },
   { "SPOT_EXPONENT"                   , 0x1205 },
   { "SPOT_CUTOFF"                     , 0x1206 },
   { "CONSTANT_ATTENUATION"            , 0x1207 },
   { "LINEAR_ATTENUATION"              , 0x1208 },
   { "QUADRATIC_ATTENUATION"           , 0x1209 },
   { "COMPILE"                         , 0x1300 },
   { "COMPILE_AND_EXECUTE"             , 0x1301 },
   { "CLEAR"                           , 0x1500 },
   { "AND"                             , 0x1501 },
   { "AND_REVERSE"                     , 0x1502 },
   { "COPY"                            , 0x1503 },
   { "AND_INVERTED"                    , 0x1504 },
   { "NOOP"                            , 0x1505 },
   { "XOR"                             , 0x1506 },
   { "OR"                              , 0x1507 },
   { "NOR"                             , 0x1508 },
   { "EQUIV"                           , 0x1509 },
   { "INVERT"                          , 0x150A },
   { "OR_REVERSE"                      , 0x150B },
   { "COPY_INVERTED"                   , 0x150C },
   { "OR_INVERTED"                     , 0x150D },
   { "NAND"                            , 0x150E },
   { "SET"                             , 0x150F },
   { "EMISSION"                        , 0x1600 },
   { "SHININESS"                       , 0x1601 },
   { "AMBIENT_AND_DIFFUSE"             , 0x1602 },
   { "COLOR_INDEXES"                   , 0x1603 },
   { "MODELVIEW"                       , 0x1700 },
   { "PROJECTION"                      , 0x1701 },
   { "TEXTURE"                         , 0x1702 },
   { "COLOR"                           , 0x1800 },
   { "DEPTH"                           , 0x1801 },
   { "STENCIL"                         , 0x1802 },
   { "COLOR_INDEX"                     , 0x1900 },
   { "STENCIL_INDEX"                   , 0x1901 },
   { "DEPTH_COMPONENT"                 , 0x1902 },
   { "RED"                             , 0x1903 },
   { "GREEN"                           , 0x1904 },
   { "BLUE"                            , 0x1905 },
   { "ALPHA"                           , 0x1906 },
   { "RGB"                             , 0x1907 },
   { "RGBA"                            , 0x1908 },
   { "LUMINANCE"                       , 0x1909 },
   { "LUMINANCE_ALPHA"                 , 0x190A },
   { "BITMAP"                          , 0x1A00 },
   { "POINT"                           , 0x1B00 },
   { "LINE"                            , 0x1B01 },
   { "FILL"                            , 0x1B02 },
   { "RENDER"                          , 0x1C00 },
   { "FEEDBACK"                        , 0x1C01 },
   { "SELECT"                          , 0x1C02 },
   { "FLAT"                            , 0x1D00 },
   { "SMOOTH"                          , 0x1D01 },
   { "KEEP"                            , 0x1E00 },
   { "REPLACE"                         , 0x1E01 },
   { "INCR"                            , 0x1E02 },
   { "DECR"                            , 0x1E03 },
   { "VENDOR"                          , 0x1F00 },
   { "RENDERER"                        , 0x1F01 },
   { "VERSION"                         , 0x1F02 },
   { "EXTENSIONS"                      , 0x1F03 },
   { "S"                               , 0x2000 },
   { "T"                               , 0x2001 },
   { "R"                               , 0x2002 },
   { "Q"                               , 0x2003 },
   { "MODULATE"                        , 0x2100 },
   { "DECAL"                           , 0x2101 },
   { "TEXTURE_ENV_MODE"                , 0x2200 },
   { "TEXTURE_ENV_COLOR"               , 0x2201 },
   { "TEXTURE_ENV"                     , 0x2300 },
   { "EYE_LINEAR"                      , 0x2400 },
   { "OBJECT_LINEAR"                   , 0x2401 },
   { "SPHERE_MAP"                      , 0x2402 },
   { "TEXTURE_GEN_MODE"                , 0x2500 },
   { "OBJECT_PLANE"                    , 0x2501 },
   { "EYE_PLANE"                       , 0x2502 },
   { "NEAREST"                         , 0x2600 },
   { "LINEAR"                          , 0x2601 },
   { "NEAREST_MIPMAP_NEAREST"          , 0x2700 },
   { "LINEAR_MIPMAP_NEAREST"           , 0x2701 },
   { "NEAREST_MIPMAP_LINEAR"           , 0x2702 },
   { "LINEAR_MIPMAP_LINEAR"            , 0x2703 },
   { "TEXTURE_MAG_FILTER"              , 0x2800 },
   { "TEXTURE_MIN_FILTER"              , 0x2801 },
   { "TEXTURE_WRAP_S"                  , 0x2802 },
   { "TEXTURE_WRAP_T"                  , 0x2803 },
   { "CLAMP"                           , 0x2900 },
   { "REPEAT"                          , 0x2901 },
   { "POLYGON_OFFSET_FACTOR"           , 0x8038 },
   { "POLYGON_OFFSET_UNITS"            , 0x2A00 },
   { "POLYGON_OFFSET_POINT"            , 0x2A01 },
   { "POLYGON_OFFSET_LINE"             , 0x2A02 },
   { "POLYGON_OFFSET_FILL"             , 0x8037 },
   { "ALPHA4"                          , 0x803B },
   { "ALPHA8"                          , 0x803C },
   { "ALPHA12"                         , 0x803D },
   { "ALPHA16"                         , 0x803E },
   { "LUMINANCE4"                      , 0x803F },
   { "LUMINANCE8"                      , 0x8040 },
   { "LUMINANCE12"                     , 0x8041 },
   { "LUMINANCE16"                     , 0x8042 },
   { "LUMINANCE4_ALPHA4"               , 0x8043 },
   { "LUMINANCE6_ALPHA2"               , 0x8044 },
   { "LUMINANCE8_ALPHA8"               , 0x8045 },
   { "LUMINANCE12_ALPHA4"              , 0x8046 },
   { "LUMINANCE12_ALPHA12"             , 0x8047 },
   { "LUMINANCE16_ALPHA16"             , 0x8048 },
   { "INTENSITY"                       , 0x8049 },
   { "INTENSITY4"                      , 0x804A },
   { "INTENSITY8"                      , 0x804B },
   { "INTENSITY12"                     , 0x804C },
   { "INTENSITY16"                     , 0x804D },
   { "R3_G3_B2"                        , 0x2A10 },
   { "RGB4"                            , 0x804F },
   { "RGB5"                            , 0x8050 },
   { "RGB8"                            , 0x8051 },
   { "RGB10"                           , 0x8052 },
   { "RGB12"                           , 0x8053 },
   { "RGB16"                           , 0x8054 },
   { "RGBA2"                           , 0x8055 },
   { "RGBA4"                           , 0x8056 },
   { "RGB5_A1"                         , 0x8057 },
   { "RGBA8"                           , 0x8058 },
   { "RGB10_A2"                        , 0x8059 },
   { "RGBA12"                          , 0x805A },
   { "RGBA16"                          , 0x805B },
   { "TEXTURE_RED_SIZE"                , 0x805C },
   { "TEXTURE_GREEN_SIZE"              , 0x805D },
   { "TEXTURE_BLUE_SIZE"               , 0x805E },
   { "TEXTURE_ALPHA_SIZE"              , 0x805F },
   { "TEXTURE_LUMINANCE_SIZE"          , 0x8060 },
   { "TEXTURE_INTENSITY_SIZE"          , 0x8061 },
   { "PROXY_TEXTURE_1D"                , 0x8063 },
   { "PROXY_TEXTURE_2D"                , 0x8064 },
   { "TEXTURE_PRIORITY"                , 0x8066 },
   { "TEXTURE_RESIDENT"                , 0x8067 },
   { "TEXTURE_BINDING_1D"              , 0x8068 },
   { "TEXTURE_BINDING_2D"              , 0x8069 },
   { "VERTEX_ARRAY"                    , 0x8074 },
   { "NORMAL_ARRAY"                    , 0x8075 },
   { "COLOR_ARRAY"                     , 0x8076 },
   { "INDEX_ARRAY"                     , 0x8077 },
   { "TEXTURE_COORD_ARRAY"             , 0x8078 },
   { "EDGE_FLAG_ARRAY"                 , 0x8079 },
   { "VERTEX_ARRAY_SIZE"               , 0x807A },
   { "VERTEX_ARRAY_TYPE"               , 0x807B },
   { "VERTEX_ARRAY_STRIDE"             , 0x807C },
   { "NORMAL_ARRAY_TYPE"               , 0x807E },
   { "NORMAL_ARRAY_STRIDE"             , 0x807F },
   { "COLOR_ARRAY_SIZE"                , 0x8081 },
   { "COLOR_ARRAY_TYPE"                , 0x8082 },
   { "COLOR_ARRAY_STRIDE"              , 0x8083 },
   { "INDEX_ARRAY_TYPE"                , 0x8085 },
   { "INDEX_ARRAY_STRIDE"              , 0x8086 },
   { "TEXTURE_COORD_ARRAY_SIZE"        , 0x8088 },
   { "TEXTURE_COORD_ARRAY_TYPE"        , 0x8089 },
   { "TEXTURE_COORD_ARRAY_STRIDE"      , 0x808A },
   { "EDGE_FLAG_ARRAY_STRIDE"          , 0x808C },
   { "VERTEX_ARRAY_POINTER"            , 0x808E },
   { "NORMAL_ARRAY_POINTER"            , 0x808F },
   { "COLOR_ARRAY_POINTER"             , 0x8090 },
   { "INDEX_ARRAY_POINTER"             , 0x8091 },
   { "TEXTURE_COORD_ARRAY_POINTER"     , 0x8092 },
   { "EDGE_FLAG_ARRAY_POINTER"         , 0x8093 },
   { "V2F"                             , 0x2A20 },
   { "V3F"                             , 0x2A21 },
   { "C4UB_V2F"                        , 0x2A22 },
   { "C4UB_V3F"                        , 0x2A23 },
   { "C3F_V3F"                         , 0x2A24 },
   { "N3F_V3F"                         , 0x2A25 },
   { "C4F_N3F_V3F"                     , 0x2A26 },
   { "T2F_V3F"                         , 0x2A27 },
   { "T4F_V4F"                         , 0x2A28 },
   { "T2F_C4UB_V3F"                    , 0x2A29 },
   { "T2F_C3F_V3F"                     , 0x2A2A },
   { "T2F_N3F_V3F"                     , 0x2A2B },
   { "T2F_C4F_N3F_V3F"                 , 0x2A2C },
   { "T4F_C4F_N3F_V4F"                 , 0x2A2D },
   { "EXT_vertex_array"                , 1 },
   { "EXT_bgra"                        , 1 },
   { "EXT_paletted_texture"            , 1 },
   { "WIN_swap_hint"                   , 1 },
   { "WIN_draw_range_elements"         , 1 },
   { "VERTEX_ARRAY_EXT"                , 0x8074 },
   { "NORMAL_ARRAY_EXT"                , 0x8075 },
   { "COLOR_ARRAY_EXT"                 , 0x8076 },
   { "INDEX_ARRAY_EXT"                 , 0x8077 },
   { "TEXTURE_COORD_ARRAY_EXT"         , 0x8078 },
   { "EDGE_FLAG_ARRAY_EXT"             , 0x8079 },
   { "VERTEX_ARRAY_SIZE_EXT"           , 0x807A },
   { "VERTEX_ARRAY_TYPE_EXT"           , 0x807B },
   { "VERTEX_ARRAY_STRIDE_EXT"         , 0x807C },
   { "VERTEX_ARRAY_COUNT_EXT"          , 0x807D },
   { "NORMAL_ARRAY_TYPE_EXT"           , 0x807E },
   { "NORMAL_ARRAY_STRIDE_EXT"         , 0x807F },
   { "NORMAL_ARRAY_COUNT_EXT"          , 0x8080 },
   { "COLOR_ARRAY_SIZE_EXT"            , 0x8081 },
   { "COLOR_ARRAY_TYPE_EXT"            , 0x8082 },
   { "COLOR_ARRAY_STRIDE_EXT"          , 0x8083 },
   { "COLOR_ARRAY_COUNT_EXT"           , 0x8084 },
   { "INDEX_ARRAY_TYPE_EXT"            , 0x8085 },
   { "INDEX_ARRAY_STRIDE_EXT"          , 0x8086 },
   { "INDEX_ARRAY_COUNT_EXT"           , 0x8087 },
   { "TEXTURE_COORD_ARRAY_SIZE_EXT"    , 0x8088 },
   { "TEXTURE_COORD_ARRAY_TYPE_EXT"    , 0x8089 },
   { "TEXTURE_COORD_ARRAY_STRIDE_EXT"  , 0x808A },
   { "TEXTURE_COORD_ARRAY_COUNT_EXT"   , 0x808B },
   { "EDGE_FLAG_ARRAY_STRIDE_EXT"      , 0x808C },
   { "EDGE_FLAG_ARRAY_COUNT_EXT"       , 0x808D },
   { "VERTEX_ARRAY_POINTER_EXT"        , 0x808E },
   { "NORMAL_ARRAY_POINTER_EXT"        , 0x808F },
   { "COLOR_ARRAY_POINTER_EXT"         , 0x8090 },
   { "INDEX_ARRAY_POINTER_EXT"         , 0x8091 },
   { "TEXTURE_COORD_ARRAY_POINTER_EXT" , 0x8092 },
   { "EDGE_FLAG_ARRAY_POINTER_EXT"     , 0x8093 },
   { "BGR_EXT"                         , 0x80E0 },
   { "BGRA_EXT"                        , 0x80E1 },
   { "COLOR_TABLE_FORMAT_EXT"          , 0x80D8 },
   { "COLOR_TABLE_WIDTH_EXT"           , 0x80D9 },
   { "COLOR_TABLE_RED_SIZE_EXT"        , 0x80DA },
   { "COLOR_TABLE_GREEN_SIZE_EXT"      , 0x80DB },
   { "COLOR_TABLE_BLUE_SIZE_EXT"       , 0x80DC },
   { "COLOR_TABLE_ALPHA_SIZE_EXT"      , 0x80DD },
   { "COLOR_TABLE_LUMINANCE_SIZE_EXT"  , 0x80DE },
   { "COLOR_TABLE_INTENSITY_SIZE_EXT"  , 0x80DF },
   { "COLOR_INDEX1_EXT"                , 0x80E2 },
   { "COLOR_INDEX2_EXT"                , 0x80E3 },
   { "COLOR_INDEX4_EXT"                , 0x80E4 },
   { "COLOR_INDEX8_EXT"                , 0x80E5 },
   { "COLOR_INDEX12_EXT"               , 0x80E6 },
   { "COLOR_INDEX16_EXT"               , 0x80E7 },
   { "MAX_ELEMENTS_VERTICES_WIN"       , 0x80E8 },
   { "MAX_ELEMENTS_INDICES_WIN"        , 0x80E9 },
   { "PHONG_WIN"                       , 0x80EA },
   { "PHONG_HINT_WIN"                  , 0x80EB },
   { "FOG_SPECULAR_TEXTURE_WIN"        , 0x80EC },
   { "CURRENT_BIT"                     , 0x00000001 },
   { "POINT_BIT"                       , 0x00000002 },
   { "LINE_BIT"                        , 0x00000004 },
   { "POLYGON_BIT"                     , 0x00000008 },
   { "POLYGON_STIPPLE_BIT"             , 0x00000010 },
   { "PIXEL_MODE_BIT"                  , 0x00000020 },
   { "LIGHTING_BIT"                    , 0x00000040 },
   { "FOG_BIT"                         , 0x00000080 },
   { "DEPTH_BUFFER_BIT"                , 0x00000100 },
   { "ACCUM_BUFFER_BIT"                , 0x00000200 },
   { "STENCIL_BUFFER_BIT"              , 0x00000400 },
   { "VIEWPORT_BIT"                    , 0x00000800 },
   { "TRANSFORM_BIT"                   , 0x00001000 },
   { "ENABLE_BIT"                      , 0x00002000 },
   { "COLOR_BUFFER_BIT"                , 0x00004000 },
   { "HINT_BIT"                        , 0x00008000 },
   { "EVAL_BIT"                        , 0x00010000 },
   { "LIST_BIT"                        , 0x00020000 },
   { "TEXTURE_BIT"                     , 0x00040000 },
   { "SCISSOR_BIT"                     , 0x00080000 },
   { "ALL_ATTRIB_BITS"                 , 0x000fffff },
   { "CLIENT_PIXEL_STORE_BIT"          , 0x00000001 },
   { "CLIENT_VERTEX_ARRAY_BIT"         , 0x00000002 },
   { "CLIENT_ALL_ATTRIB_BITS"          , 0xffffffff },
   { 0, 0}
};

#define BUILDING_LUAGL_DLL

/* set field of a lua table with a number */
static void set_field(lua_State *L, unsigned int index, lua_Number value)
{
   lua_pushnumber(L, index);
   lua_pushnumber(L, value);
   lua_settable(L, -3);
}

static GLenum get_enum(const char *str, int n)
{
   int i = 0;

   while(gl_str[i].str != 0)
   {
      if(strncmp(str, gl_str[i].str, n) == 0 && gl_str[i].str[n] == 0)
         return gl_str[i].value;

      i++;
   }
   return ENUM_ERROR;
}
static GLenum get_gl_enum(lua_State *L, int index)
{
   unsigned int i;
   const char *str = lua_tostring(L, index);
   GLenum temp = 0, ret = 0;

   for(i = 0; i < strlen(str); i++)
   {
      if(str[i] == ',')
      {
         temp = get_enum(str, i);
         if(temp != ENUM_ERROR)
            ret |= temp;

         str += i+1;
         i = 0;
      }
   }
   temp = get_enum(str, strlen(str));

   if(temp == ENUM_ERROR)
   {
      if(ret == 0)
         return ENUM_ERROR;
      return ret;
   }

   return ret | temp;
}

static const char *get_str_gl_enum(GLenum num)
{
   unsigned int i = 0;

   while(gl_str[i].str != 0)
   {
      if(num == gl_str[i].value)
         return gl_str[i].str;

      i++;
   }
   return NULL;
}

/* Gets an array from a lua table, store it in 'array' and returns the no. of elems of the array
   index refers to where the table is in stack. */
static int get_arrayb(lua_State *L, int index, GLboolean **array)
{
   int i;
   int n = luaL_getn(L, index);

   *array = (GLboolean *)malloc(n * sizeof(GLboolean));
   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i + 1);
      (*array)[i] = (GLboolean)lua_toboolean(L, -1);
   }

   return n; /* return the number of valid elements found.*/
}
static int get_arrayd(lua_State *L, int index, GLdouble **array)
{
   int i;
   int n = luaL_getn(L, index);

   *array = (GLdouble *)malloc(n * sizeof(GLdouble));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i + 1);
      (*array)[i] = (GLdouble)lua_tonumber(L, -1);
   }

   return n; /* return the number of valid elements found.*/
}
static int get_arrayf(lua_State *L, int index, GLfloat **array)
{
   int i;
   int n = luaL_getn(L, index);

   *array = (GLfloat *)malloc(n * sizeof(GLfloat));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i + 1);
      (*array)[i] = (GLfloat)lua_tonumber(L, -1);
   }

   return n; /* return the number of valid elements found.*/
}
static int get_arrayui(lua_State *L, int index, GLuint **array)
{
   int i;
   int n = luaL_getn(L, index);

   *array = (GLuint *)malloc(n * sizeof(GLint));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i + 1);
      (*array)[i] = (GLuint)lua_tonumber(L, -1);
   }

   return n; /* return the number of valid elements found.*/
}
static int get_arrayubyte(lua_State *L, int index, GLubyte **array)
{
   int i;
   int n = luaL_getn(L, index);

   *array = (GLubyte *)malloc(n * sizeof(GLubyte));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i + 1);
      (*array)[i] = (GLubyte)lua_tonumber(L, -1);
   }

   return n; /* return the number of valid elements found.*/
}
static int get_array2ubyte(lua_State *L, int index, GLubyte **array, int *size)
{
   int i, j;
   int n = luaL_getn(L, index);

   lua_rawgeti(L, index, 1);

   if(!lua_istable(L, -1))
   {
      lua_remove(L, -1);
      return -1;
   }

   *size = luaL_getn(L, -1);

   *array = (GLubyte *)malloc(n * (*size) * sizeof(GLubyte));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i+1);

      if(!lua_istable(L, -1))
         return -1;

      for(j = 0; j < *size; j++)
      {
         lua_rawgeti(L, -1, j + 1);

         (*array)[i*(*size) + j] = (GLubyte)lua_tonumber(L, -1);

         lua_remove(L, -1);
      }
   }

   return n; /* return the number of valid elements found.*/
}

static int get_array2d(lua_State *L, int index, GLdouble **array, int *size)
{
   int i, j;
   int n = luaL_getn(L, index);

   lua_rawgeti(L, index, 1);

   if(!lua_istable(L, -1))
   {
      lua_remove(L, -1);
      return -1;
   }

   *size = luaL_getn(L, -1);

   *array = (GLdouble *)malloc(n * (*size) * sizeof(GLdouble));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i+1);

      if(!lua_istable(L, -1))
         return -1;

      for(j = 0; j < *size; j++)
      {
         lua_rawgeti(L, -1, j + 1);

         (*array)[i*(*size) + j] = (GLdouble)lua_tonumber(L, -1);

         lua_remove(L, -1);
      }
   }

   return n; /* return the number of valid elements found.*/
}
static int get_array2f(lua_State *L, int index, GLfloat **array, int *size)
{
   int i, j;
   int n = luaL_getn(L, index);

   lua_rawgeti(L, index, 1);

   if(!lua_istable(L, -1))
   {
      lua_remove(L, -1);
      return -1;
   }

   *size = luaL_getn(L, -1);

   *array = (GLfloat *)malloc(n * (*size) * sizeof(GLfloat));

   for(i = 0; i < n; i++)
   {
      lua_rawgeti(L, index, i+1);

      if(!lua_istable(L, -1))
         return -1;

      for(j = 0; j < *size; j++)
      {
         lua_rawgeti(L, -1, j + 1);

         (*array)[i*(*size) + j] = (GLfloat)lua_tonumber(L, -1);

         lua_remove(L, -1);
      }
   }

   return n; /* return the number of valid elements found.*/
}

static int str2mask(const char *str)
{
   int i, j;
   int mask = 0;
   int size = strlen(str);
   for(i = 0, j = 0; j < size; i++)
   {
      if(str[i] == '1')
      {
         mask |= (1 << (size-1-j));
         j++;
      }
      else if(str[i] == '0')
         j++;

   }
   return mask;
}
static const char *mask2str(int mask)
{
   unsigned int i;
   static char str[17];
   for(i = 0; i < 16; i++)
   {
      if(mask & (1 << (15 - i)))
         str[i] = '1';
      else
         str[i] = '0';
   }
   str[i] = 0;
   return str;
}

/*Accum (op, value) -> none*/
static int gl_accum(lua_State *L)
{
   /* get string parameters */
   GLenum e;

   /* test argument */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Accum'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Accum'");

   if(!lua_isnumber(L, 2))
      luaL_error(L, "incorrect argument to function 'gl.Accum'");

   /* call opengl function */
   glAccum(e, (GLfloat)lua_tonumber(L, 2));

   return 0;
}

/*AlphaFunc (func, ref) -> none*/
static int gl_alpha_func(lua_State *L)
{
   /* get string parameters */
   GLenum e;

   /* test argument */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.AlphaFunc'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.AlphaFunc'");

   if(!lua_isnumber(L, 2))
      luaL_error(L, "incorrect argument to function 'gl.AlphaFunc'");

   /* call opengl function */
   glAlphaFunc(e, (GLclampf)lua_tonumber(L, 2));

   return 0;
}

/*AreTexturesResident (texturesArray) -> residences*/
static int gl_are_textures_resident(lua_State *L)
{
   GLboolean *residences;
   GLuint *textures;

   int i, n;

   /* test argument */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.AreTexturesResident'");

   /* get textures array */
   n = get_arrayui(L, 1, &textures);

   residences = (GLboolean *)malloc(n * sizeof(GLboolean));

   /* call opengl function */
   glAreTexturesResident(n, (GLuint *)textures, residences);

   lua_newtable(L);

   /* return residences values */
   for(i = 0; i < n; i++)
      set_field(L, i+1, residences[i]);

   free(textures);
   free(residences);

   return 1;
}

/*ArrayElement (i) -> none*/
static int gl_array_element(lua_State *L)
{
   /* test argument */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ArrayElement'");

   /* call opengl function */
   glArrayElement((GLint)lua_tonumber(L, 1));

   return 0;
}

/*Begin (mode) -> none*/
static int gl_begin(lua_State *L)
{
   GLenum e;

   /* test argument */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Begin'");

   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Begin'");

   /* call opengl function */
   glBegin(e);

   return 0;
}

/*BindTexture (target, texture) -> none*/
static int gl_bind_texture(lua_State *L)
{
   GLenum e;

   /* test arguments */
   if(!( lua_isstring(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.BindTexture'");

   /* get string value */
   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.BindTexture'");

   /* call opengl function */
   glBindTexture(e, (GLuint)lua_tonumber(L, 2));

   return 0;
}

/*Bitmap (xorig, yorig, ymove, bitmap) -> none*/
static int gl_bitmap(lua_State *L)
{
   int width, height;

   GLubyte *bitmap;

   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_istable(L,5) ))
      luaL_error(L, "incorrect argument to function 'gl.Bitmap'");

   if((height = get_array2ubyte(L, 4, &bitmap, &width)) == -1)
      luaL_error(L, "incorrect argument to function 'gl.Bitmap'");

   glBitmap(width, height, (GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
            (GLfloat)lua_tonumber(L, 3), (GLfloat)lua_tonumber(L, 4), bitmap);
   return 0;
}

/*BlendFunc (sfactor, dfactor) -> none*/
static int gl_blend_func(lua_State *L)
{
   GLenum a, b;

   /* test arguments */
   if(!(lua_isstring(L, 1) && lua_isstring(L, 2)))
      luaL_error(L, "incorrect argument to function 'gl.BlendFunc'");

   /* get values */
   a = (GLenum)get_gl_enum(L, 1);
   b = (GLenum)get_gl_enum(L, 2);

   /* test arguments */
   if((a == ENUM_ERROR) || (b == ENUM_ERROR))
      luaL_error(L, "incorrect string argument to function 'gl.BlendFunc'");

   /* call opengl function */
   glBlendFunc(a, b);

   return 0;
}

/*CallList (list) -> none*/
static int gl_call_list(lua_State *L)
{
   /* test argument */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.CallList'");

   /* call opengl function */
   glCallList((GLuint)lua_tonumber(L, 1));

   return 0;
}

/*CallLists (listArray) -> none*/
static int gl_call_lists(lua_State *L)
{
   GLsizei n;
   GLfloat *lists;

   /* test argument */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.CallLists'");

   /* get array of lists */
   n = get_arrayf(L, 1, &lists);

   /* call opengl function */
   glCallLists(n, GL_FLOAT, lists);

   free(lists);

   return 0;
}

/*Clear (mask) -> none*/
static int gl_clear(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Clear'");

   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Clear'");

   /* call opengl function */
   glClear(e);

   return 0;
}

/*ClearAccum (red, green, blue, alpha) -> none*/
static int gl_clear_accum(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.ClearAccum'");

   /* call opengl function */
   glClearAccum((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2),
                (GLfloat)lua_tonumber(L, 3), (GLfloat)lua_tonumber(L, 4));

   return 0;
}

/*ClearColor (red, green, blue, alpha) -> none*/
static int gl_clear_color(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.ClearColor'");

   /* call opengl function */
   glClearColor((GLclampf)lua_tonumber(L, 1), (GLclampf)lua_tonumber(L, 2),
                (GLclampf)lua_tonumber(L, 3), (GLclampf)lua_tonumber(L, 4));

   return 0;
}

/*ClearDepth (depth) -> none*/
static int gl_clear_depth(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ClearDepth'");

   /* call opengl function */
   glClearDepth((GLclampd)lua_tonumber(L, 1));

   return 0;
}

/*ClearIndex (c) -> none*/
static int gl_clear_index(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ClearIndex'");

   /* call opengl function */
   glClearIndex((GLfloat)lua_tonumber(L, 1));

   return 0;
}

/*ClearStencil (s) -> none*/
static int gl_clear_stencil(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ClearStencil'");

   /* call opengl function */
   glClearStencil((GLint)lua_tonumber(L, 1));

   return 0;
}

/*ClipPlane (plane, equationArray) -> none*/
static int gl_clip_plane(lua_State *L)
{
   GLenum plane;
   GLdouble *equation;

   /* test arguments */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ClipPlane'");

   if(!lua_istable(L, 2))
      luaL_error(L, "incorrect argument to function 'gl.ClipPlane'");

   /* get values */
   plane = get_gl_enum(L, 1);

   /* test argument */
   if(plane == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.ClipPlane'");

   /* get array of equations */
   get_arrayd(L, 2, &equation);

   /* call opengl function */
   glClipPlane(plane, equation);

   free(equation);

   return 0;
}

/*Color (red, green, blue [, alpha]) -> none
  Color (color) -> none*/
static int gl_color(lua_State *L)
{
   GLdouble *array = 0;

   int index;
   int num_args = lua_gettop(L);

   /* test arguments type */
   if(lua_istable(L, 1))
   {
      num_args = get_arrayd(L, 1, &array);

      /* if more then 4 arguments, ignore the others */
      if(num_args > 4)
         num_args = 4;

      /* call openGL functions */
      switch(num_args)
      {
         case 3:  glColor3dv(array); break;
         case 4:  glColor4dv(array); break;
      }

      if(array)
         free(array);

      return 0;
   }

   /* if more then 4 arguments, ignore the others */
   if(num_args > 4)
      num_args = 4;

   for(index = 0; index < num_args; index++)
   {
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.Color'");
   }

   /* call openGL functions */
   switch(num_args)
   {
      case 3:  glColor3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                         (GLdouble)lua_tonumber(L, 3));
               break;
      case 4:  glColor4d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                         (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));
               break;
   }
   return 0;
}

/*ColorMask (red, green, blue, alpha) -> none*/
static int gl_color_mask(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isboolean(L, 1) && lua_isboolean(L, 2) && lua_isboolean(L, 3) && lua_isboolean(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.ColorMask'");

   glColorMask((GLboolean)lua_toboolean(L, 1), (GLboolean)lua_toboolean(L, 2),
               (GLboolean)lua_toboolean(L, 3), (GLboolean)lua_toboolean(L, 4));

   return 0;
}

/*ColorMaterial (face, mode) -> none*/
static int gl_color_material(lua_State *L)
{
   GLenum e1, e2;

   /* test arguments */
   if(!( lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.ColorMaterial'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test strings */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.ColorMaterial'");

   /* call opengl function */
   glColorMaterial(e1, e2);

   return 0;
}

/*ColorPointer (colorArray) -> none*/
static int gl_color_pointer(lua_State *L)
{
   GLint size;
   static GLdouble *array = 0;
   if(array)
      free(array);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ColorPointer'");

   if(lua_isnumber(L, 2))
   {
      size = (GLint)lua_tonumber(L, 2);
      get_arrayd(L, 1, &array);
   }
   else if(get_array2d(L, 1, &array, &size) == -1)
   {
      luaL_error(L, "incorrect argument to function 'gl.ColorPointer'");
      return 0;
   }

   /* call opengl function */
   glColorPointer(size, GL_DOUBLE, 0, array);

   return 0;
}

/*CopyPixels (x, y, width, height, type) -> none*/
static int gl_copy_pixels(lua_State *L)
{
   GLenum e;

   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isstring(L, 5) ))
      luaL_error(L, "incorrect argument to function 'gl.CopyPixels'");

   /* get string parameter */
   e = get_gl_enum(L, 5);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.CopyPixels'");

   /* call opengl function */
   glCopyPixels((GLint)  lua_tonumber(L, 1), (GLint)  lua_tonumber(L, 2),
                (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4), (GLenum)e);

   return 0;
}

/*CopyTexImage (level, internalFormat, border, x, y, width[, height]) -> none*/
static int gl_copy_tex_image(lua_State *L)
{
   GLenum internalFormat;

   int num_args = lua_gettop(L);

   /* test arguments type */
   if(!(lua_isnumber(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3) &&
        lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6) ))
      luaL_error(L, "incorrect argument to function 'gl.CopyTexImage'");

   /* get string parameter */
   internalFormat = get_gl_enum(L, 2);

   /* test argument */
   if(internalFormat == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.CopyTexImage'");

   /* call opengl functions */
   if (num_args > 6 && lua_isnumber(L, 7))
   {
      glCopyTexImage2D(GL_TEXTURE_2D, (GLint)lua_tonumber(L, 1), internalFormat,
                       (GLint)lua_tonumber(L, 4), (GLint)lua_tonumber(L, 5),
                       (GLsizei)lua_tonumber(L, 6), (GLsizei)lua_tonumber(L, 7),
                       (GLint)lua_tonumber(L, 3));
   }
   else
   {
      glCopyTexImage1D(GL_TEXTURE_1D, (GLint)lua_tonumber(L, 1), internalFormat,
                       (GLint)lua_tonumber(L, 4), (GLint)lua_tonumber(L, 5),
                       (GLsizei)lua_tonumber(L, 6), (GLint)lua_tonumber(L, 3));
   }
   return 0;
}

/*CopyTexSubImage (level, x, y, xoffset, width[, yoffset, height]) -> none*/
static int gl_copy_tex_sub_image(lua_State *L)
{
   int index;
   int num_args = lua_gettop(L);

   /* test arguments type */
   for(index = 0; index < num_args; index++)
   {
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.CopyTexSubImage'");
   }

   /* call opengl funcitions */
   if(num_args >= 7)
   {
      glCopyTexSubImage2D(GL_TEXTURE_2D,
                          (GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 4),
                          (GLint)lua_tonumber(L, 6), (GLint)lua_tonumber(L, 2),
                          (GLint)lua_tonumber(L, 3), (GLint)lua_tonumber(L, 5),
                          (GLint)lua_tonumber(L, 7));
   }
   else
   {
      glCopyTexSubImage1D(GL_TEXTURE_1D,
                          (GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 4),
                          (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3),
                          (GLint)lua_tonumber(L, 5));
   }
   return 0;
}

/*CullFace (mode) -> none*/
static int gl_cull_face(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.CullFace'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.CullFace'");

   /* call opengl function */
   glCullFace(e);

   return 0;
}

/*DeleteLists (list, range) -> none*/
static int gl_delete_lists(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.DeleteLists'");

   /* call opengl function */
   glDeleteLists((GLuint)lua_tonumber(L, 1), (GLsizei)lua_tonumber(L, 2));

   return 0;
}

/*DeleteTextures (texturesArray) -> none*/
static int gl_delete_textures(lua_State *L)
{
   int n;
   GLuint *textures;

   /* test argument type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.DeleteTextures'");

   /* get textures array */
   n = get_arrayui(L, 1, &textures);

   /* call opengl function */
   glDeleteTextures((GLsizei)n, (GLuint *)textures);

   free(textures);

   return 0;
}

/*DepthFunc (func) -> none*/
static int gl_depth_func(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.DepthFunc'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.DepthFunc'");

   /* call opengl function */
   glDepthFunc(e);

   return 0;
}

/*DepthMask (flag) -> none*/
static int gl_depth_mask(lua_State *L)
{
   /* test argument type */
   if(!lua_isboolean(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.DepthMask'");

   /* call opengl function */
   glDepthMask((GLboolean)lua_toboolean(L, 1));

   return 0;
}

/*DepthRange (zNear, zFar) -> none*/
static int gl_depth_range(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.DepthRange'");

   /* call opengl function */
   glDepthRange((GLclampd)lua_tonumber(L, 1), (GLclampd)lua_tonumber(L, 2));

   return 0;
}

/*Disable (cap) -> none*/
static int gl_disable(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Disable'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Disable'");

   /* call opengl function */
   glDisable(e);

   return 0;
}

/*DisableClientState (array) -> none*/
static int gl_disable_client_state(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.DisableClientState'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.DisableClientState'");

   /* call opengl function */
   glDisableClientState(e);

   return 0;
}

/*DrawArrays (mode, first, count) -> none*/
static int gl_draw_arrays(lua_State *L)
{
   GLenum e;

   /* test arguments type */
   if(!(lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.DrawArrays'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.DrawArrays'");

   /* call opengl function */
   glDrawArrays(e, (GLint)lua_tonumber(L, 2), (GLsizei)lua_tonumber(L, 3));

   return 0;
}

/*DrawBuffer (mode) -> none*/
static int gl_draw_buffer(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.DrawBuffer'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.DrawBuffer'");

   /* call opengl function */
   glDrawBuffer(e);

   return 0;
}

/*DrawElements (mode, indicesArray) -> none*/
static int gl_draw_elements(lua_State *L)
{
   int n;
   GLuint *indices;
   GLenum e;

   /* test arguments type */
   if(!( lua_isstring(L, 1) && lua_istable(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.DrawElements'");

   /* get parameters */
   e = get_gl_enum(L, 1);
   n = get_arrayui(L, 2, &indices);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.DrawElements'");

   /* call opengl function */
   glDrawElements(e, n, GL_UNSIGNED_INT, indices);

   free(indices);

   return 0;
}

/*DrawPixels (width, height, format, pixels) -> none*/
// replace by our version in 'gl' namespace
// static int gl_draw_pixels(lua_State *L)
// {
//    GLenum e;
//    GLfloat *pixels;
//
//    /* test arguments type */
//    if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
//         lua_isstring(L, 3) && lua_istable (L, 4)) )
//       luaL_error(L, "incorrect argument to function 'gl.DrawPixels'");
//
//    /* get parameters */
//    e = get_gl_enum(L, 3);
//    get_arrayf(L, 4, &pixels);
//
//    /* test argument */
//    if(e == ENUM_ERROR)
//       luaL_error(L, "incorrect string argument to function 'gl.DrawPixels'");
//
//    /* call opengl function */
//    glDrawPixels((GLsizei)lua_tonumber(L, 1), (GLsizei)lua_tonumber(L, 2), e, GL_FLOAT, pixels);
//
//    free(pixels);
//
//    return 0;
// }

/*EdgeFlag (flag) -> none*/
static int gl_edge_flag(lua_State *L)
{
   GLboolean *flag;

   if(lua_istable(L, 1))/* test argument type */
   {
      /* get argument */
      get_arrayb(L, 1, &flag);

      /* call opengl function */
      glEdgeFlagv((GLboolean *)flag);

      free(flag);
   }
   else if(lua_isboolean(L, 1))/* test argument type */
      /* call opengl function */
      glEdgeFlag((GLboolean)lua_toboolean(L, 1));

   else
      luaL_error(L, "incorrect argument to function 'gl.EdgeFlag'");

   return 0;
}

/*EdgeFlagPointer (flagsArray) -> none*/
static int gl_edge_flag_pointer(lua_State *L)
{
   static GLboolean *flags = 0;
   if(flags)
      free(flags);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.EdgeFlagPointer'");

   /* get argument */
   get_arrayb(L, 1, &flags);

   /* call opengl function */
   glEdgeFlagPointer(0, flags);

   return 0;
}

/*Enable (cap) -> none*/
static int gl_enable(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Enable'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Enable'");

   /* call opengl function */
   glEnable(e);
   return 0;
}

/*EnableClientState (array) -> none*/
static int gl_enable_client_state(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.EnableClientState'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.EnableClientState'");

   /* call opengl function */
   glEnableClientState(e);

   return 0;
}

/*End () -> none*/
static int gl_end(lua_State *L)
{
   glEnd();
   return 0;
}

/*EndList () -> none*/
static int gl_end_list(lua_State *L)
{
   glEndList();
   return 0;
}

/*EvalCoord (u[, v]) -> none
  EvalCoord (coordArray) -> none*/
static int gl_eval_coord(lua_State *L)
{
   GLdouble *array;

   int index;
   int num_args = lua_gettop(L);

   /* test arguments type */
   if(lua_istable(L, 1))
   {
      /* get_array and return no of elements */
      if(get_arrayd(L, 1, &array) == 1)
      {
         glEvalCoord1dv(array);
         return 0;
      }
      else
         glEvalCoord2dv(array);

      free(array);

      return 0;
   }

   /* if more then 2 arguments, ignore the others */
   if(num_args > 2)
      num_args = 2;

   /* test arguments */
   for(index = 0; index < num_args; index++)
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.EvalCoord'");

   /* call openGL functions */
   switch(num_args)
   {
      case 1:  glEvalCoord1d((GLdouble)lua_tonumber(L, 1));
               break;
      case 2:  glEvalCoord2d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2));
               break;
   }
   return 0;
}

/*EvalMesh (mode, i1, i2[,j1, j2]) -> none*/
static int gl_eval_mesh(lua_State *L)
{
   GLenum e;

   int index;
   int num_args = lua_gettop(L);

   /* test arguments type */
   if(!( lua_isstring(L, 1) && num_args > 2))
      luaL_error(L, "incorrect argument to function 'gl.EvalMesh'");

   for(index = 2; index < num_args; index++)
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.EvalMesh'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.EvalMesh'");

   /* call opengl function */
   if(num_args < 5)
      glEvalMesh1(e, (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3));
   else
      glEvalMesh2(e, (GLint)lua_tonumber(L, 2), (GLint)lua_tonumber(L, 3),
                     (GLint)lua_tonumber(L, 4), (GLint)lua_tonumber(L, 5));

   return 0;
}

/*EvalPoint (i[, j]) -> none*/
static int gl_eval_point(lua_State *L)
{
   int index;
   int num_args = lua_gettop(L);

   /* if more then 2 arguments, ignore the others */
   if(num_args > 2)
      num_args = 2;

   /* test arguments */
   for(index = 0; index < num_args; index++)
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.EvalPoint'");

   /* call openGL functions */
   if(num_args == 1)
      glEvalPoint1((GLint)lua_tonumber(L, 1));
   else
      glEvalPoint2((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2));

   return 0;
}

/*FeedbackBuffer (size, type) -> dataArray*/
static int gl_feedback_buffer(lua_State *L)
{
   GLfloat *array;
   GLenum e;
   GLsizei size;
   int i;

   if(!( lua_isnumber(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.FeedbackBuffer'");

   /* get parameters */
   size = (GLsizei)lua_tonumber(L, 1);
   e = get_gl_enum(L, 2);

   array = (GLfloat *)malloc(size * sizeof(GLfloat));

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.FeedbackBuffer'");

   /* call opengl function */
   glFeedbackBuffer (size, e, array);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, array[i]);

   free(array);

   return 0;
}

/*Finish () -> none*/
static int gl_finish(lua_State *L)
{
   glFinish();
   return 0;
}

/*Flush () -> none*/
static int gl_flush(lua_State *L)
{
   glFlush();
   return 0;
}

/*Fog (pname, param) -> none
  Fog (pname, paramsArray) -> none*/
static int gl_fog(lua_State *L)
{
   GLenum e;
   GLfloat *param;

   /* test first argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Fog'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Fog'");

   if(lua_istable(L, 2))
   {
      get_arrayf(L, 2, &param);

      /* call opengl function */
      glFogfv(e, (GLfloat*)param);

      free(param);

      return 0;
   }
   /* test second argument */
   else if(lua_isnumber(L, 2))
   {
      /* call opengl function */
      glFogf(e, (GLfloat)lua_tonumber(L, 2));
   }
   else if(lua_isstring(L, 2))
   {
      /* call opengl function */
      glFogi(e, get_gl_enum(L, 2));
   }
   else
      luaL_error(L, "incorrect argument to function 'gl.Fog'");

   return 0;
}

/*FrontFace (mode) -> none*/
static int gl_front_face(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.FrontFace'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.FrontFace'");

   /* call opengl function */
   glFrontFace(e);

   return 0;
}

/*Frustum (left, right, bottom, top, zNear, zFar) -> none*/
static int gl_frustum(lua_State *L)
{
   int index;

   /* test arguments type */
   for(index = 0; index < 6; index++)
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.Frustum'");

   /* call opengl function */
   glFrustum((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
             (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4),
             (GLdouble)lua_tonumber(L, 5), (GLdouble)lua_tonumber(L, 6));

   return 0;
}

/*GenLists (range) -> num*/
static int gl_gen_lists(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GenLists'");

   /* call opengl function and push the return value on the stack */
   lua_pushnumber(L, glGenLists((GLsizei)lua_tonumber(L, 1)) );

   return 1;
}

/*GenTextures (n) -> texturesArray*/
static int gl_gen_textures(lua_State *L)
{
   int i;
   GLsizei n;
   GLuint *textures;

   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GenTextures'");

   n = (GLsizei)lua_tonumber(L, 1);
   textures = (GLuint *)malloc(n * sizeof(GLuint));

   /* call opengl function */
   glGenTextures(n, (GLuint *)textures);

   lua_newtable(L);

   for(i = 0; i < n; i++)
      set_field(L, i+1, textures[i]);

   free(textures);

   return 1;
}

/*Get (pname) -> params*/
static int gl_get(lua_State *L)
{
   int i, size=1;
   GLenum e;
   GLdouble *params;
   int mask;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.Get'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   switch(e)
   {
      case GL_STENCIL_VALUE_MASK:
      case GL_LINE_STIPPLE_PATTERN:
      case GL_STENCIL_WRITEMASK:
      case GL_INDEX_WRITEMASK:
         /* call opengl function */
         mask = 0;
         glGetIntegerv(e, &mask);
         lua_pushstring(L, mask2str(mask));
         return 1;

      case GL_DEPTH_RANGE:
      case GL_MAP1_GRID_DOMAIN:
      case GL_MAP2_GRID_SEGMENTS:
      case GL_MAX_VIEWPORT_DIMS:
      case GL_POINT_SIZE_RANGE:
      case GL_POLYGON_MODE:
         size = 2;
         break;

      case GL_CURRENT_NORMAL:
         size = 3;
         break;

      case GL_ACCUM_CLEAR_VALUE:
      case GL_COLOR_CLEAR_VALUE:
      case GL_COLOR_WRITEMASK:
      case GL_CURRENT_COLOR:
      case GL_CURRENT_RASTER_COLOR:
      case GL_CURRENT_RASTER_POSITION:
      case GL_CURRENT_RASTER_TEXTURE_COORDS:
      case GL_CURRENT_TEXTURE_COORDS:
      case GL_FOG_COLOR:
      case GL_LIGHT_MODEL_AMBIENT:
      case GL_MAP2_GRID_DOMAIN:
      case GL_SCISSOR_BOX:
      case GL_TEXTURE_ENV_COLOR:
      case GL_VIEWPORT:
         size = 4;
         break;

      case GL_MODELVIEW_MATRIX:
      case GL_PROJECTION_MATRIX:
      case GL_TEXTURE_MATRIX:
         size = 16;
         break;

      case ENUM_ERROR:
         luaL_error(L, "incorrect string argument to function 'gl.Get'");
         break;
   }
   params = (GLdouble *)malloc(size * sizeof(GLdouble));

   /* call opengl function */
   glGetDoublev(e, params);

   for(i = 0; i < size; i++)
      lua_pushnumber(L, params[i]);

   free(params);

   return size;
}

/*GetConst (pname) -> constant string*/
static int gl_get_const(lua_State *L)
{
   int i, size=1;
   GLenum e;
   GLenum *params;
   const char *str;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetConst'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   switch(e)
   {
      case GL_DEPTH_RANGE:
      case GL_MAP1_GRID_DOMAIN:
      case GL_MAP2_GRID_SEGMENTS:
      case GL_MAX_VIEWPORT_DIMS:
      case GL_POINT_SIZE_RANGE:
      case GL_POLYGON_MODE:
         size = 2;
         break;

      case GL_CURRENT_NORMAL:
         size = 3;
         break;

      case GL_ACCUM_CLEAR_VALUE:
      case GL_COLOR_CLEAR_VALUE:
      case GL_COLOR_WRITEMASK:
      case GL_CURRENT_COLOR:
      case GL_CURRENT_RASTER_COLOR:
      case GL_CURRENT_RASTER_POSITION:
      case GL_CURRENT_RASTER_TEXTURE_COORDS:
      case GL_CURRENT_TEXTURE_COORDS:
      case GL_FOG_COLOR:
      case GL_LIGHT_MODEL_AMBIENT:
      case GL_MAP2_GRID_DOMAIN:
      case GL_SCISSOR_BOX:
      case GL_TEXTURE_ENV_COLOR:
      case GL_VIEWPORT:
         size = 4;
         break;

      case GL_MODELVIEW_MATRIX:
      case GL_PROJECTION_MATRIX:
      case GL_TEXTURE_MATRIX:
         size = 16;
         break;
   }
   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetConst'");

   params = (GLenum *)malloc(size * sizeof(GLenum));

   /* call opengl function */
   glGetIntegerv(e, (GLint*)params);

   for(i = 0; i < size; i++)
   {
      str = get_str_gl_enum(params[i]);
      lua_pushstring(L, str);
   }

   free(params);

   return size;
}

/*GetArray (pname) -> paramsArray*/
static int gl_get_array(lua_State *L)
{
   int i, size = 1;
   GLenum e;
   GLdouble *params;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetArray'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetArray'");

   switch(e)
   {
      case GL_DEPTH_RANGE:
      case GL_MAP1_GRID_DOMAIN:
      case GL_MAP2_GRID_SEGMENTS:
      case GL_MAX_VIEWPORT_DIMS:
      case GL_POINT_SIZE_RANGE:
      case GL_POLYGON_MODE:
         size = 2;
         break;

      case GL_CURRENT_NORMAL:
         size = 3;
         break;

      case GL_ACCUM_CLEAR_VALUE:
      case GL_COLOR_CLEAR_VALUE:
      case GL_COLOR_WRITEMASK:
      case GL_CURRENT_COLOR:
      case GL_CURRENT_RASTER_COLOR:
      case GL_CURRENT_RASTER_POSITION:
      case GL_CURRENT_RASTER_TEXTURE_COORDS:
      case GL_CURRENT_TEXTURE_COORDS:
      case GL_FOG_COLOR:
      case GL_LIGHT_MODEL_AMBIENT:
      case GL_MAP2_GRID_DOMAIN:
      case GL_SCISSOR_BOX:
      case GL_TEXTURE_ENV_COLOR:
      case GL_VIEWPORT:
         size = 4;
         break;

      case GL_MODELVIEW_MATRIX:
      case GL_PROJECTION_MATRIX:
      case GL_TEXTURE_MATRIX:
         size = 16;
         break;
   }

   params = (GLdouble *)malloc(size * sizeof(GLdouble));

   /* call opengl function */
   glGetDoublev(e, params);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, params[i]);

   free(params);

   return 1;
}

/*GetClipPlane (plane) -> equationArray*/
static int gl_get_clip_plane(lua_State *L)
{
   int i;
   GLenum e;
   GLdouble *equation;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetClipPlane'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetClipPlane'");

   equation = (GLdouble *)malloc(4 * sizeof(GLdouble));

   /* call opengl function */
   glGetClipPlane(e, equation);

   lua_newtable(L);

   for(i = 0; i < 4; i++)
      set_field(L, i+1, equation[i]);

   free(equation);

   return 1;
}

/*GetError () -> error flag*/
static int gl_get_error(lua_State *L)
{
   /* call glGetError function,
      convert returned number to string,
      and push the string on the stack. */
   GLenum error = glGetError();

   if(error == GL_NO_ERROR)
      lua_pushstring(L, "NO_ERROR");
   else
      lua_pushstring(L, get_str_gl_enum(error));

   return 1;
}

/*GetLight (light, pname) -> paramsArray*/
static int gl_get_light(lua_State *L)
{
   int i, size = 1;
   GLenum e1, e2;
   GLfloat *params;

   /* test arguments type */
   if(!( lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.GetLight'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetLight'");

   switch(e2)
   {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_POSITION:
         size = 4;
         break;
      case GL_SPOT_DIRECTION :
         size = 3;
         break;
      case GL_SPOT_EXPONENT:
      case GL_SPOT_CUTOFF:
      case GL_CONSTANT_ATTENUATION:
      case GL_LINEAR_ATTENUATION:
      case GL_QUADRATIC_ATTENUATION:
         size = 1;
         break;
   }

   params = (GLfloat *)malloc(size * sizeof(GLfloat));

   /* call opengl function */
   glGetLightfv(e1, e2, params);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, params[i]);

   free(params);

   return 1;
}

/*GetMap (target, query) -> vArray*/
static int gl_get_map(lua_State *L)
{
   int i, size = 1;
   GLenum e1, e2;
   GLdouble *params;
   GLint *order;

   order = (GLint *)malloc(2 * sizeof(GLint));
   order[0] = order[1] = 1;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.GetMap'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetMap'");

   switch(e1)
   {
      case GL_MAP1_INDEX:
      case GL_MAP2_INDEX:
      case GL_MAP1_TEXTURE_COORD_1:
      case GL_MAP2_TEXTURE_COORD_1:
         size = 1;
         break;
      case GL_MAP1_TEXTURE_COORD_2:
      case GL_MAP2_TEXTURE_COORD_2:
         size = 2;
         break;
      case GL_MAP1_VERTEX_3:
      case GL_MAP2_VERTEX_3:
      case GL_MAP1_NORMAL:
      case GL_MAP2_NORMAL:
      case GL_MAP1_TEXTURE_COORD_3:
      case GL_MAP2_TEXTURE_COORD_3:
         size = 3;
         break;
      case GL_MAP1_VERTEX_4:
      case GL_MAP2_VERTEX_4:
      case GL_MAP1_COLOR_4:
      case GL_MAP2_COLOR_4:
      case GL_MAP1_TEXTURE_COORD_4:
      case GL_MAP2_TEXTURE_COORD_4:
         size = 4;
         break;
   }

   glGetMapiv(e1, GL_ORDER, order);

   size *= order[0] * order[1];

   params = (GLdouble *)malloc(size * sizeof(GLdouble));

   /* call opengl function */
   glGetMapdv(e1, e2, params);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, params[i]);

   free(params);

   return 1;
}

/*GetMaterial (face, pname) -> paramsArray*/
static int gl_get_material(lua_State *L)
{
   int i, size = 1;
   GLenum e1, e2;
   GLfloat *params;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.GetMaterial'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   switch(e2)
   {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_EMISSION:
         size = 4;
         break;
      case GL_COLOR_INDEXES:
         size = 3;
         break;
      case GL_SHININESS:
         size = 1;
         break;
   }

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetMaterial'");

   params = (GLfloat *)malloc(size * sizeof(GLfloat));

   /* call opengl function */
   glGetMaterialfv(e1, e2, params);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, params[i]);

   free(params);

   return 1;
}

/*GetPixelMap (map) -> valuesArray*/
static int gl_get_pixel_map(lua_State *L)
{
   int size;
   int i, s = GL_PIXEL_MAP_R_TO_R_SIZE;
   GLenum e;
   GLfloat *values;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetPixelMap'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetPixelMap'");

   switch(e)
   {
      case GL_PIXEL_MAP_I_TO_I: s = GL_PIXEL_MAP_I_TO_I_SIZE; break;
      case GL_PIXEL_MAP_S_TO_S: s = GL_PIXEL_MAP_S_TO_S_SIZE; break;
      case GL_PIXEL_MAP_I_TO_R: s = GL_PIXEL_MAP_I_TO_R_SIZE; break;
      case GL_PIXEL_MAP_I_TO_G: s = GL_PIXEL_MAP_I_TO_G_SIZE; break;
      case GL_PIXEL_MAP_I_TO_B: s = GL_PIXEL_MAP_I_TO_B_SIZE; break;
      case GL_PIXEL_MAP_I_TO_A: s = GL_PIXEL_MAP_I_TO_A_SIZE; break;
      case GL_PIXEL_MAP_R_TO_R: s = GL_PIXEL_MAP_R_TO_R_SIZE; break;
      case GL_PIXEL_MAP_G_TO_G: s = GL_PIXEL_MAP_G_TO_G_SIZE; break;
      case GL_PIXEL_MAP_B_TO_B: s = GL_PIXEL_MAP_B_TO_B_SIZE; break;
      case GL_PIXEL_MAP_A_TO_A: s = GL_PIXEL_MAP_A_TO_A_SIZE; break;
   }
   glGetIntegerv(s, &size);

   values = (GLfloat *)malloc(size * sizeof(GLfloat));

   /* call opengl function */
   glGetPixelMapfv(e, values);

   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, values[i]);

   free(values);

   return 1;
}

/*GetPointer (pname, n) -> valuesArray*/
static int gl_get_pointer(lua_State *L)
{
   int i, n;
   GLenum e;
   GLboolean *flags;
   GLdouble *params;

   /* test argument type */
   if(!( lua_isstring(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.GetPointer'");

   e = get_gl_enum(L, 1);
   n = (int)lua_tonumber(L, 2);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetPointer'");

   if(e == GL_EDGE_FLAG_ARRAY_POINTER)
   {
      flags = (GLboolean *)malloc(n * sizeof(GLboolean));

      /* call opengl function */
      glGetPointerv(e, (GLvoid**)&flags);

      if(flags == 0)
         return 0;

      lua_newtable(L);

      for(i = 0; i < n ; i++)
         set_field(L, i+1, flags[i]);
   }
   else
   {
      params = (GLdouble *)malloc(n * sizeof(GLdouble));

      /* call opengl function */
      glGetPointerv(e, (GLvoid**)&params);

      if(params == 0)
         return 0;

      lua_newtable(L);

      for(i = 0; i < n ; i++)
         set_field(L, i+1, params[i]);
   }

   return 1;
}

/*GetPolygonStipple () -> maskArray*/
static int gl_get_polygon_stipple(lua_State *L)
{
   int index;
   GLubyte *mask = (GLubyte*)malloc(32*32 * sizeof(GLubyte));

   glGetPolygonStipple(mask);

   lua_newtable(L);

   for(index = 0; index < 1024; index++)
      set_field(L, index+1, mask[index]);

   return 1;
}

/*GetString (name) -> string*/
static int gl_get_string(lua_State *L)
{
   GLenum e;
   const GLubyte *str;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetString'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetString'");

   /* call opengl function */
   str = glGetString(e);

   lua_pushstring(L, (const char*)str);

   return 1;
}

/*GetTexEnv (pname) -> paramsArray*/
static int gl_get_tex_env(lua_State *L)
{
   int i;
   GLenum e1;
   GLfloat *params;
   int e2;

   /* test arguments type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.GetTexEnv'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);

   /* test argument */
   if(e1 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetTexEnv'");

   if(e1 == GL_TEXTURE_ENV_MODE)
   {
      glGetTexEnviv(GL_TEXTURE_ENV, e1, &e2);

      lua_pushstring(L, get_str_gl_enum(e2));
   }
   else if(e1 == GL_TEXTURE_ENV_COLOR)
   {
      params = (GLfloat *)malloc(4 * sizeof(GLfloat));

      /* call opengl function */
      glGetTexEnvfv(GL_TEXTURE_ENV, e1, params);

      lua_newtable(L);

      for(i = 0; i < 4; i++)
         set_field(L, i+1, params[i]);

      free(params);
   }
   else
   {
      luaL_error(L, "incorrect string argument to function 'gl.GetTexEnv'");
   }
   return 1;
}

/*GetTexGen (coord, pname) -> paramsArray*/
static int gl_get_tex_gen(lua_State *L)
{
   int i;
   GLenum e1, e2;
   GLdouble *params;
   int e3;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.GetTexGen'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.GetTexGen'");
   if(e2 == GL_TEXTURE_GEN_MODE)
   {
      /* call opengl function */
      glGetTexGeniv(e1, e2, &e3);

      lua_pushstring(L, get_str_gl_enum(e3));
   }
   else
   {
      params = (GLdouble *)malloc(4 * sizeof(GLdouble));

      /* call opengl function */
      glGetTexGendv(e1, e2, params);

      lua_newtable(L);

      for(i = 0; i < 4; i++)
         set_field(L, i+1, params[i]);

      free(params);
   }
   return 1;
}

/*GetTexImage (target, level, format) -> pixelsArray*/
static int gl_get_tex_image(lua_State *L)
{
   int i, n=1;
   int width, height, level;
   GLenum target, format;
   GLfloat *pixels;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3)) )
      luaL_error(L, "incorrect argument to function 'gl.GetTexImage'");

   /* get string parameters */
   target = get_gl_enum(L, 1);
   level = (int)lua_tonumber(L, 2);
   format = get_gl_enum(L, 3);

   /* get width and height of image */
   glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &width);
   glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &height);

   switch(format)
   {
      case GL_RED:  case GL_GREEN:  case GL_BLUE:
      case GL_ALPHA: case GL_LUMINANCE: n = 1; break;
      case GL_LUMINANCE_ALPHA:          n = 2; break;
      case GL_RGB:  case GL_BGR_EXT:    n = 3; break;
      case GL_RGBA: case GL_BGRA_EXT:   n = 4; break;
      default:
         luaL_error(L, "incorrect string argument to function 'gl.GetTexImage'");
   }

   pixels = (GLfloat *)malloc(n * width * height * sizeof(GLfloat));

   /* call opengl function */
   glGetTexImage(target, level, format, GL_FLOAT, pixels);

   lua_newtable(L);

   for(i = 0; i < n * width * height; i++)
      set_field(L, i+1, pixels[i]);

   free(pixels);

   return 1;
}

/*GetTexLevelParameter (target, level, pname) -> param*/
static int gl_get_tex_level_parameter(lua_State *L)
{
   int level;
   GLenum target, pname;
   GLfloat param;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3)) )
      luaL_error(L, "incorrect argument to function 'gl.GetTexLevelParameter'");

   /* get parameters */
   target = get_gl_enum(L, 1);
   level = (int)lua_tonumber(L, 2);
   pname = get_gl_enum(L, 3);

   /* call opengl function */
   glGetTexLevelParameterfv(target, level, pname, &param);

   /* return parameter */
   lua_pushnumber(L, param);

   return 1;
}

/*GetTexParameter (target, pname) -> paramsArray*/
static int gl_get_tex_parameter(lua_State *L)
{
   int i;
   GLenum target, pname;
   GLfloat *params;
   GLfloat param;
   int e;

   /* test arguments type */
   if(! (lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.GetTexParameter'");

   /* get string parameters */
   target = get_gl_enum(L, 1);
   pname = get_gl_enum(L, 2);

   if(pname == GL_TEXTURE_BORDER_COLOR)
   {
      params = (GLfloat *)malloc(4 * sizeof(float));

      /* call opengl function */
      glGetTexParameterfv(target, pname, params);

      /* return parameters */
      lua_newtable(L);

      for(i = 0; i < 4; i++)
         set_field(L, i+1, params[i]);
   }
   else if(pname == GL_TEXTURE_PRIORITY)
   {
      /* call opengl function */
      glGetTexParameterfv(target, pname, &param);

      lua_pushnumber(L, param);
   }
   else
   {
      /* call opengl function */
      glGetTexParameteriv(target, pname, &e);

      lua_pushstring(L, get_str_gl_enum(e));
   }
   return 1;
}

/*Hint (target, mode) -> none*/
static int gl_hint(lua_State *L)
{
   GLenum e1, e2;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.Hint'");

   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Hint'");

   /* call opengl function */
   glHint(e1, e2);

   return 0;
}

/*Index (c) -> none*/
static int gl_index(lua_State *L)
{
   GLdouble *c;

   if(lua_istable(L, 1))/* test argument type */
   {
      /* get argument */
      get_arrayd(L, 1, &c);

      /* call opengl function */
      glIndexdv((GLdouble *)c);

      free(c);
   }
   else if(lua_isnumber(L, 1))/* test argument type */
      /* call opengl function */
      glIndexd((GLdouble)lua_tonumber(L, 1));

   else
      luaL_error(L, "incorrect argument to function 'gl.Index'");

   return 0;
}

/*IndexMask (mask) -> none*/
static int gl_index_mask(lua_State *L)
{
   if(lua_type(L,1) == LUA_TSTRING)
      /* call opengl function */
      glIndexMask(str2mask(lua_tostring(L, 1)));

   else if(lua_type(L,1) == LUA_TNUMBER)
      /* call opengl function */
      glIndexMask((GLuint)lua_tonumber(L, 1));

   else
      luaL_error(L, "incorrect argument to function 'gl.IndexMask'");

   return 0;
}

/*IndexPointer (indexArray) -> none*/
static int gl_index_pointer(lua_State *L)
{
   static GLdouble *array = 0;
   if(array)
      free(array);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.IndexPointer'");

   /* get argument */
   get_arrayd(L, 1, &array);

   /* call opengl function */
   glIndexPointer(GL_DOUBLE, 0, array);

   return 0;
}

/*InitNames () -> none*/
static int gl_init_names(lua_State *L)
{
   glInitNames();
   return 0;
}

/*IsEnabled (cap) -> true/false*/
static int gl_is_enabled(lua_State *L)
{
   GLenum e;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.IsEnabled'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.IsEnabled'");

   /* call opengl function */
   lua_pushboolean(L, glIsEnabled(e));

   return 1;
}

/*IsList (list) -> true/false*/
static int gl_is_list(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.IsList'");

   /* call opengl function and push return value in the lua stack */
   lua_pushboolean(L, glIsList((GLuint)lua_tonumber(L, 1)));

   return 1;
}

/*IsTexture (texture) -> true/false*/
static int gl_is_texture(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.IsTexture'");

   /* call opengl function and push return value in the lua stack */
   lua_pushboolean(L, glIsTexture((GLuint)lua_tonumber(L, 1)));

   return 1;
}

/*Light (light, pname, param) -> none
  Light (light, pname, paramsArray) -> none*/
static int gl_light(lua_State *L)
{
   GLenum e1, e2;
   GLfloat *params;

   /* test arguments type */
   if(!( lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.Light'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Light'");

   /* test argument type */
   if(lua_istable(L, 3))
   {
      /* get argument */
      get_arrayf(L, 3, &params);

      /* call opengl function */
      glLightfv(e1, e2, (GLfloat *)params);

      free(params);
   }
   /* test argument type */
   else if(lua_isnumber(L, 3))
   {
      /* call opengl function */
      glLightf(e1, e2, (GLfloat)lua_tonumber(L, 3));
   }
   else
      luaL_error(L, "incorrect argument to function 'gl.Light'");

   return 0;
}

/*LightModel (pname, param) -> none
  LightModel (pname, paramsArray) -> none*/
static int gl_light_model(lua_State *L)
{
   GLenum e;
   GLfloat *params;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.LightModel'");

   /* get string parameter */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.LightModel'");

   /* test argument type */
   if(lua_istable(L, 2))
   {
      /* get argument */
      get_arrayf(L, 2, &params);

      /* call opengl function */
      glLightModelfv(e, (GLfloat *)params);

      free(params);
   }
   /* test argument type */
   else if(lua_isnumber(L, 2))
      /* call opengl function */
      glLightModelf(e, (GLfloat)lua_tonumber(L, 2));

   else
      luaL_error(L, "incorrect argument to function 'gl.LightModel'");

   return 0;
}

/*LineStipple (factor, pattern) -> none*/
static int gl_line_stipple(lua_State *L)
{
   /* test arguments type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.LineStipple'");

   if(lua_type(L,2) == LUA_TSTRING)
      /* call opengl function */
      glLineStipple((GLint)lua_tonumber(L, 1), (GLushort)str2mask(lua_tostring(L, 2)));

   else if(lua_type(L,2) == LUA_TNUMBER)
      /* call opengl function */
      glLineStipple((GLint)lua_tonumber(L, 1), (GLushort)lua_tonumber(L, 2));

   else
      luaL_error(L, "incorrect argument to function 'gl.LineStipple'");

   return 0;
}

/*LineWidth (width) -> none*/
static int gl_line_width(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.LineWidth'");

   /* call opengl function */
   glLineWidth((GLfloat)lua_tonumber(L, 1));

   return 0;
}

/*ListBase (base) -> none*/
static int gl_list_base(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ListBase'");

   /* call opengl function */
   glListBase((GLuint)lua_tonumber(L, 1));

   return 0;
}

/*LoadIdentity () -> none*/
static int gl_load_identity(lua_State *L)
{
   glLoadIdentity();
   return 0;
}

/*LoadMatrix (mArray) -> none*/
static int gl_load_matrix(lua_State *L)
{
   GLdouble *m;

   /* test argument type and the number of arguments in the array, must be 16 values */
   if(!lua_istable(L, 1) || luaL_getn(L, 1) < 16)
      luaL_error(L, "incorrect argument to function 'gl.LoadMatrix'");

   /* get argument */
   get_arrayd(L, 1, &m);

   /* call opengl function */
   glLoadMatrixd(m);

   free(m);

   return 0;
}

/*LoadName (name) -> none*/
static int gl_load_name(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.LoadName'");

   /* call opengl function */
   glLoadName((GLuint)lua_tonumber(L, 1));

   return 0;
}

/*LogicOp (opcode) -> none*/
static int gl_logic_op(lua_State *L)
{
   GLenum opcode;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.LogicOp'");

   /* get string parameter */
   opcode = get_gl_enum(L, 1);

   /* test argument */
   if(opcode == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.LogicOp'");

   /* call opengl function */
   glLogicOp(opcode);

   return 0;
}

/*Map (target, u1, u2, pointsArray) -> none
  Map (target, u1, u2, ustride, v1, v2, vstride, pointsArray) -> none*/
static int gl_map(lua_State *L)
{
   int size=1;
   GLenum target;
   GLdouble *points;
   GLint uorder, vorder;

   /* test argument */
   if(!( lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.Map'");

   target = get_gl_enum(L, 1);

   switch(target)
   {
      case GL_MAP1_INDEX:
      case GL_MAP2_INDEX:
      case GL_MAP1_TEXTURE_COORD_1:
      case GL_MAP2_TEXTURE_COORD_1:
         size = 1;
         break;
      case GL_MAP1_TEXTURE_COORD_2:
      case GL_MAP2_TEXTURE_COORD_2:
         size = 2;
         break;
      case GL_MAP1_VERTEX_3:
      case GL_MAP2_VERTEX_3:
      case GL_MAP1_NORMAL:
      case GL_MAP2_NORMAL:
      case GL_MAP1_TEXTURE_COORD_3:
      case GL_MAP2_TEXTURE_COORD_3:
         size = 3;
         break;
      case GL_MAP1_VERTEX_4:
      case GL_MAP2_VERTEX_4:
      case GL_MAP1_COLOR_4:
      case GL_MAP2_COLOR_4:
      case GL_MAP1_TEXTURE_COORD_4:
      case GL_MAP2_TEXTURE_COORD_4:
         size = 4;
         break;
   }

   /* test argument */
   if(target == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Map'");

   /* test number of argument in the array */
   if(lua_gettop(L) < 6)
   {
      if(!lua_istable(L, 4))
         luaL_error(L, "incorrect argument to function 'gl.Map' (argument 4 should be a list of points)");

      /* get argument */
      uorder = get_arrayd(L, 4, &points) / size;

      /* call opengl function */
      glMap1d(target, (GLdouble)lua_tonumber(L, 2),
                      (GLdouble)lua_tonumber(L, 3),
                      size, uorder, points);

      free(points);
   }
   else
   {
      if(!( lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_istable(L, 6) ))
         luaL_error(L, "incorrect argument to function 'gl.Map'");

      /* get argument */
      vorder = get_array2d(L, 6, &points, &uorder);
      uorder /= size;

      /* call opengl function */
      glMap2d(target, (GLdouble)lua_tonumber(L, 2),
                      (GLdouble)lua_tonumber(L, 3),
                      size, uorder,
                      (GLdouble)lua_tonumber(L, 4),
                      (GLdouble)lua_tonumber(L, 5),
                      size * uorder, vorder,
                      points);

      free(points);
   }
   return 0;
}

/*MapGrid (un, u1, u2[, vn, v1, v2]) -> none*/
static int gl_map_grid(lua_State *L)
{
   /* test arguments */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.MapGrid'");

   /* test number of arguments */
   if(lua_gettop(L) < 6)
   {
      /* call opengl function */
      glMapGrid1d((GLint)lua_tonumber(L, 1),
                  (GLdouble)lua_tonumber(L, 2),
                  (GLdouble)lua_tonumber(L, 3));
   }
   else
   {
      /* test arguments type */
      if(!( lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6) ))
         luaL_error(L, "incorrect argument to function 'gl.MapGrid'");

      /* call opengl function */
      glMapGrid2d((GLint)lua_tonumber(L, 1),
                  (GLdouble)lua_tonumber(L, 2),
                  (GLdouble)lua_tonumber(L, 3),
                  (GLint)lua_tonumber(L, 4),
                  (GLdouble)lua_tonumber(L, 5),
                  (GLdouble)lua_tonumber(L, 6));
   }
   return 0;
}

/*Material (face, pname, param) -> none*/
static int gl_material(lua_State *L)
{
   GLenum e1, e2;
   GLfloat *params;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.Material'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.Material'");

   /* test argument type */
   if(lua_istable(L, 3))
   {
      /* get argument */
      get_arrayf(L, 3, &params);

      /* call opengl function */
      glMaterialfv(e1, e2, (GLfloat *)params);

      free(params);
   }
   /* test argument type */
   else if(lua_isnumber(L, 3))
   {
      /* call opengl function */
      glMaterialf(e1, e2, (GLfloat)lua_tonumber(L, 3));
   }
   else
      luaL_error(L, "incorrect argument to function 'gl.Material'");

   return 0;
}

/*MatrixMode (mode) -> none*/
static int gl_matrix_mode(lua_State *L)
{
   GLenum mode;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.MatrixMode'");

   /* get string parameter */
   mode = get_gl_enum(L, 1);

   /* test argument */
   if(mode == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.MatrixMode'");

   /* call opengl function */
   glMatrixMode(mode);

   return 0;
}

/*MultMatrix (mArray) -> none*/
static int gl_mult_matrix(lua_State *L)
{
   GLdouble *m;

   /* test argument type and the number of arguments in the array, must be 16 values */
   if(!lua_istable(L, 1) || luaL_getn(L, 1) < 16)
      luaL_error(L, "incorrect argument to function 'gl.MultMatrix'");

   /* get argument */
   get_arrayd(L, 1, &m);

   /* call opengl function */
   glMultMatrixd((GLdouble *)m);

   free(m);

   return 0;
}

/*NewList (list, mode) -> none*/
static int gl_new_list(lua_State *L)
{
   GLenum e;

   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.NewList'");

   /* get string parameter */
   e = get_gl_enum(L, 2);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.NewList'");

   /* call opengl function */
   glNewList((GLint)lua_tonumber(L, 1), e);

   return 0;
}

/*Normal (nx, ny, nz) -> none
  Normal (nArray) -> none*/
static int gl_normal(lua_State *L)
{
   GLdouble *array;

   int num_args;

   /* test arguments type */
   if(lua_istable(L, 1))
   {
      num_args = get_arrayd(L, 1, &array);

      if(num_args < 3)
         luaL_error(L, "incorrect argument to function 'gl.Normal'");

      /* call openGL function */
      glNormal3dv(array);

      free(array);

      return 0;
   }

   /* test arguments */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.Normal'");

   /* call openGL functions */
   glNormal3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
              (GLdouble)lua_tonumber(L, 3));

   return 0;
}

/*NormalPointer (normalArray) -> none*/
static int gl_normal_pointer(lua_State *L)
{
   GLint size;

   static GLdouble *array = 0;

   if(array)
      free(array);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.NormalPointer'");
   /* get argument */
   if(get_array2d(L, 1, &array, &size) == -1)
      size = get_arrayd(L, 1, &array) / 3;

   /* call opengl function */
   glNormalPointer(GL_DOUBLE, 0, array);

   return 0;
}

/*Ortho (left, right, bottom, top, zNear, zFar) -> none*/
static int gl_ortho(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) &&
         lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6)))
      luaL_error(L, "incorrect string argument to function 'gl.Ortho'");

   /* call opengl function */
   glOrtho((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
           (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4),
           (GLdouble)lua_tonumber(L, 5), (GLdouble)lua_tonumber(L, 6));

   return 0;
}

/*Ortho2D (left, right, bottom, top) -> none*/
static int gl_ortho2D(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4)))
      luaL_error(L, "incorrect string argument to function 'gl.Ortho2D'");

   /* call opengl function */
   gluOrtho2D((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
              (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));

   return 0;
}

/*PassThrough (token) -> none*/
static int gl_pass_through(lua_State *L)
{
   /* test argument type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect string argument to function 'gl.PassThrough'");

   /* call opengl function */
   glPassThrough((GLfloat)lua_tonumber(L, 1));

   return 0;
}

/*PixelMap (map, valuesArray) -> none*/
static int gl_pixel_map(lua_State *L)
{
   GLenum map;
   GLfloat *values;
   int mapsize;

   /* test arguments */
   if(!( lua_isstring(L, 1) && lua_istable(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.PixelMap'");

   /* get values */
   map = get_gl_enum(L, 1);

   /* test argument */
   if(map == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PixelMap'");

   /* get array of equations */
   mapsize = get_arrayf(L, 2, &values);

   /* call opengl function */
   glPixelMapfv(map, mapsize, values);

   free(values);

   return 0;
}

/*PixelStore (pname, param) -> none*/
static int gl_pixel_store(lua_State *L)
{
   /* get string parameters */
   GLenum e;

   /* test argument */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PixelStore'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PixelStore'");

   if(lua_isnumber(L, 2))
      /* call opengl function */
      glPixelStoref(e, (GLfloat)lua_tonumber(L, 2));

   else if(lua_isboolean(L,2))
      /* call opengl function */
      glPixelStoref(e, (GLfloat)lua_toboolean(L, 2));

   else
      luaL_error(L, "incorrect argument to function 'gl.PixelStore'");

   return 0;
}

/*PixelTransfer (pname, param) -> none*/
static int gl_pixel_transfer(lua_State *L)
{
   /* get string parameters */
   GLenum e;

   /* test argument */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PixelTransfer'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PixelTransfer'");

   if(lua_isnumber(L, 2))
      /* call opengl function */
      glPixelTransferf(e, (GLfloat)lua_tonumber(L, 2));

   else if(lua_isboolean(L,2))
      /* call opengl function */
      glPixelTransferf(e, (GLfloat)lua_toboolean(L, 2));

   else
      luaL_error(L, "incorrect argument to function 'gl.PixelTransfer'");

   return 0;
}

/*PixelZoom (xfactor, yfactor) -> none*/
static int gl_pixel_zoom(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect string argument to function 'gl.PixelZoom'");

   /* call opengl function */
   glPixelZoom((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2));

   return 0;
}

/*PointSize (size) -> none*/
static int gl_point_size(lua_State *L)
{
   /* test arguments type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect string argument to function 'gl.PointSize'");

   /* call opengl function */
   glPointSize((GLfloat)lua_tonumber(L, 1));

   return 0;
}

/*PolygonMode (face, mode) -> none*/
static int gl_polygon_mode(lua_State *L)
{
   GLenum e1, e2;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2)) )
      luaL_error(L, "incorrect argument to function 'gl.PolygonMode'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PolygonMode'");

   /* call opengl function */
   glPolygonMode(e1, e2);

   return 0;
}

/*PolygonOffset (factor, units) -> none*/
static int gl_polygon_offset(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect string argument to function 'gl.PolygonOffset'");

   /* call opengl function */
   glPolygonOffset((GLfloat)lua_tonumber(L, 1), (GLfloat)lua_tonumber(L, 2));

   return 0;
}

/*PolygonStipple (maskArray) -> none*/
static int gl_polygon_stipple(lua_State *L)
{
   GLubyte *array;
   int width, height = 32;

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PolygonStipple'");

   if((height = get_array2ubyte(L, 1, &array, &width)) == -1)
      width = get_arrayubyte(L, 4, &array);

   if(width != 32 && height != 32)
   {
      free(array);
      luaL_error(L, "incorrect argument to function 'gl.PolygonStipple'");
   }

   /* call opengl function */
   glPolygonStipple(array);

   return 0;
}

/*PopAttrib () -> none*/
static int gl_pop_attrib(lua_State *L)
{
   glPopAttrib();
   return 0;
}

/*PopClientAttrib () -> none*/
static int gl_pop_client_attrib(lua_State *L)
{
   glPopClientAttrib();
   return 0;
}

/*PopMatrix () -> none*/
static int gl_pop_matrix(lua_State *L)
{
   glPopMatrix();
   return 0;
}

/*PopName () -> none*/
static int gl_pop_name(lua_State *L)
{
   glPopName();
   return 0;
}

/*PrioritizeTextures (texturesArray, prioritiesArray) -> none*/
static int gl_prioritize_textures(lua_State *L)
{
   GLsizei n1, n2;
   GLuint *array1;
   GLclampf *array2;

   /* test arguments type */
   if(!( lua_istable(L, 1) && lua_istable(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.PrioritizeTextures'");

   /* get arguments */
   n1 = get_arrayui(L, 1, &array1);
   n2 = get_arrayf(L, 2, &array2);

   /* call opengl function */
   if(n1 > n2) n1 =  n2;

   glPrioritizeTextures(n1, array1, array2);

   free(array1);
   free(array2);

   return 0;
}

/*PushAttrib (mask) -> none*/
static int gl_push_attrib(lua_State *L)
{
   GLbitfield e;

   /* test arguments type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PushAttrib'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PushAttrib'");

   /* call opengl function */
   glPushAttrib(e);

   return 0;
}

/*PushClientAttrib (mask) -> none*/
static int gl_push_client_attrib(lua_State *L)
{
   GLbitfield e;

   /* test arguments type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PushClientAttrib'");

   e = get_gl_enum(L, 1);

   /* test arguments */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.PushClientAttrib'");

   /* call opengl function */
   glPushClientAttrib(e);

   return 0;
}

/*PushMatrix () -> none*/
static int gl_push_matrix(lua_State *L)
{
   glPushMatrix();
   return 0;
}

/*PushName (GLuint name) -> none*/
static int gl_push_name(lua_State *L)
{
   /* test arguments type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.PushName'");

   /* call opengl function */
   glPushName((GLuint)lua_tonumber(L, 1));

   return 0;
}

/*RasterPos (x, y[, z, w]) -> none
  RasterPos (vArray) -> none*/
static int gl_raster_pos(lua_State *L)
{
   GLdouble *array;

   int index;
   int num_args = lua_gettop(L);

   /* test arguments type */
   if(lua_istable(L, 1))
   {
      num_args = get_arrayd(L, 1, &array);

      /* if more then 4 arguments, ignore the others */
      if(num_args > 4)
         num_args = 4;

      /* call openGL functions */
      switch(num_args)
      {
         case 2:  glRasterPos2dv(array); break;
         case 3:  glRasterPos3dv(array); break;
         case 4:  glRasterPos4dv(array); break;
      }

      free(array);

      return 0;
   }

   /* if more then 4 arguments, ignore the others */
   if(num_args > 4)
      num_args = 4;

   for(index = 0; index < num_args; index++)
   {
      if(!lua_isnumber(L, index + 1))
         luaL_error(L, "incorrect argument to function 'gl.RasterPos'");
   }

   /* call openGL functions */
   switch(num_args)
   {
      case 2:  glRasterPos2d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2));
               break;
      case 3:  glRasterPos3d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                             (GLdouble)lua_tonumber(L, 3));
               break;
      case 4:  glRasterPos4d((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                             (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));
               break;
   }
   return 0;
}

/*ReadBuffer (mode) -> none*/
static int gl_read_buffer(lua_State *L)
{
   GLenum mode;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ReadBuffer'");

   /* get string parameter */
   mode = get_gl_enum(L, 1);

   /* test argument */
   if(mode == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.ReadBuffer'");

   /* call opengl function */
   glReadBuffer(mode);

   return 0;
}

/*ReadPixels (x, y, width, height, format, pixelsArray) -> none*/
static int gl_read_pixels(lua_State *L)
{
   GLenum e;
   GLfloat *pixels;

   /* test arguments type */
   if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
        lua_isnumber(L, 3) && lua_isnumber(L, 4) &&
        lua_isstring(L, 5) && lua_istable (L, 6)) )
      luaL_error(L, "incorrect argument to function 'gl.ReadPixels'");

   /* get parameters */
   e = get_gl_enum(L, 5);
   get_arrayf(L, 6, &pixels);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.ReadPixels'");

   /* call opengl function */
   glReadPixels((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
                (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4),
                e, GL_FLOAT, pixels);

   free(pixels);

   return 0;
}

/*Rect (x1, y1, x2, y2) -> none
  Rect (v1, v2) -> none*/
static int gl_rect(lua_State *L)
{
   GLdouble *v1, *v2;

   /* test argument type */
   if(lua_istable(L, 1) && lua_istable(L, 2))
   {
      /* get parameters */
      get_arrayd(L, 1, &v1);
      get_arrayd(L, 2, &v2);

      /* call opengl function */
      glRectdv(v1, v2);

      free(v1);
      free(v2);
   }
   /* test argument type */
   else if(lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
           lua_isnumber(L, 3) && lua_isnumber(L, 4))
      /* call openGL functions */
      glRectd((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
              (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));

   else
      luaL_error(L, "incorrect argument to function 'gl.Rect'");

   return 0;
}

/*RenderMode (mode) -> none*/
static int gl_render_mode(lua_State *L)
{
   GLenum mode;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.RenderMode'");

   /* get string parameter */
   mode = get_gl_enum(L, 1);

   /* test argument */
   if(mode == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.RenderMode'");

   /* call opengl function */
   glRenderMode(mode);

   return 0;
}

/*Rotate (angle, x, y, z) -> none*/
static int gl_rotate(lua_State *L)
{
   /* test argument type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.Rotate'");

   /* call opengl function */
   glRotated((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
             (GLdouble)lua_tonumber(L, 3), (GLdouble)lua_tonumber(L, 4));

   return 0;
}

/*Scale (x, y, z) -> none*/
static int gl_scale(lua_State *L)
{
   /* test argument type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.Scale'");

   /* call opengl function */
   glScaled((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
            (GLdouble)lua_tonumber(L, 3));

   return 0;
}

/*Scissor (x, y, width, height) -> none*/
static int gl_scissor(lua_State *L)
{
   /* test argument type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.Scissor'");

   /* call opengl function */
   glScissor((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
             (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4));

   return 0;
}

/*SelectBuffer (size) -> SelectArray*/
static int gl_select_buffer(lua_State *L)
{
   int size, i;
   GLuint *buffer;

   /* test arguments type */
   if(!lua_isnumber(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.SelectBuffer'");

   size = (int)lua_tonumber(L, 1);

   buffer = (GLuint *)malloc(size * sizeof(GLuint));

   /* call opengl function */
   glSelectBuffer (size, buffer);

   /* return parameters */
   lua_newtable(L);

   for(i = 0; i < size; i++)
      set_field(L, i+1, buffer[i]);

   free(buffer);

   return 1;
}

/*ShadeModel (mode) -> none*/
static int gl_shade_model(lua_State *L)
{
   GLenum mode;

   /* test argument type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.ShadeModel'");

   /* get string parameter */
   mode = get_gl_enum(L, 1);

   /* test argument */
   if(mode == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.ShadeModel'");

   /* call opengl function */
   glShadeModel(mode);

   return 0;
}

/*StencilFunc (func, ref, mask) -> none*/
static int gl_stencil_func(lua_State *L)
{
   GLenum func;

   /* test arguments type */
   if(!( lua_isstring(L, 1) && lua_isnumber(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.StencilFunc'");

   /* get string parameter */
   func = get_gl_enum(L, 1);

   /* test argument */
   if(func == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.StencilFunc'");

   if(lua_type(L,3) == LUA_TSTRING)
      /* call opengl function */
      glStencilFunc(func, (GLint)lua_tonumber(L, 2), str2mask(lua_tostring(L, 3)));

   else if(lua_type(L,3) == LUA_TNUMBER)
      /* call opengl function */
      glStencilFunc(func, (GLint)lua_tonumber(L, 2), (GLuint)lua_tonumber(L, 3));

   else
      luaL_error(L, "incorrect argument to function 'gl.StencilFunc'");

   return 0;
}

/*StencilMask (mask) -> none*/
static int gl_stencil_mask(lua_State *L)
{
   if(lua_type(L,1) == LUA_TSTRING)
      /* call opengl function */
      glStencilMask(str2mask(lua_tostring(L, 1)));

   else if(lua_type(L,1) == LUA_TNUMBER)
      /* call opengl function */
      glStencilMask((GLuint)lua_tonumber(L, 1));

   else
      luaL_error(L, "incorrect argument to function 'gl.StencilMask'");

   return 0;
}

/*StencilOp (fail, zfail, zpass) -> none*/
static int gl_stencil_op(lua_State *L)
{
   GLenum e1, e2, e3;

   /* test arguments type */
   if( !(lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.StencilOp'");

   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);
   e3 = get_gl_enum(L, 3);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR || e3 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.StencilOp'");

   /* call opengl function */
   glStencilOp(e1, e2, e3);

   return 0;
}

/*TexCoord (s[, t, r, q]) -> none
  TexCoord (vArray) -> none*/
static int gl_tex_coord(lua_State *L)
{
   int index;
   int num_args = lua_gettop(L);

   GLdouble *v = 0;

   /* if more then 4 arguments, ignore the others */
   if(num_args > 4)
      num_args = 4;

   /* if have there's no arguments show an error message */
   if(num_args == 0)
      luaL_error(L, "incorrect argument to function 'gl.TexCoord'");

   /* test argument type */
   if(lua_istable(L, 1))
      num_args = get_arrayd(L, 1, &v);

   else
   {
      v = (GLdouble *)malloc(num_args * sizeof(GLdouble));

      /* get arguments */
      for(index = 0; index < num_args; index++)
      {
         /* test arguments type */
         if(!lua_isnumber(L, index + 1))
            luaL_error(L, "incorrect argument to function 'gl.TexCoord'");

         /* get argument */
         v[index] = lua_tonumber(L, index + 1);
      }
   }

   /* call openGL functions */
   switch(num_args)
   {
      case 1:  glTexCoord1dv((GLdouble *)v);  break;
      case 2:  glTexCoord2dv((GLdouble *)v);  break;
      case 3:  glTexCoord3dv((GLdouble *)v);  break;
      case 4:  glTexCoord4dv((GLdouble *)v);  break;
      default: break;
   }

   free(v);

   return 0;
}

/*TexCoordPointer(vArray) -> none*/
static int gl_tex_coord_pointer(lua_State *L)
{
   GLint size;
   static GLdouble *array = 0;

   if(array)
      free(array);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.TexCoordPointer'");

   if(lua_isnumber(L, 2))
   {
      size = (GLint)lua_tonumber(L, 2);
      get_arrayd(L, 1, &array);
   }
   else if(get_array2d(L, 1, &array, &size) == -1)
      luaL_error(L, "incorrect argument to function 'gl.TexCoordPointer'");

   /* call opengl function */
   glTexCoordPointer(size, GL_DOUBLE, 0, array);

   return 0;
}

/*TexEnv (pname, param) -> none
  TexEnv (pname, paramsArray) -> none*/
int static gl_tex_env(lua_State *L)
{
   GLfloat *param;
   GLenum e;

   /* test arguments type */
   if(!lua_isstring(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.TexEnv'");

   /* get string parameters */
   e = get_gl_enum(L, 1);

   /* test argument */
   if(e == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.TexEnv'");

   if(lua_istable(L, 2))
   {
      get_arrayf(L, 2, &param);

      /* call opengl function */
      glTexEnvfv(GL_TEXTURE_ENV, e, (GLfloat *)param);

      free(param);
   }
   else if(lua_isnumber(L, 2))
      /* call opengl function */
      glTexEnvf(GL_TEXTURE_ENV, e, (GLfloat)lua_tonumber(L, 2));

   else if(lua_isstring(L, 2))
      /* call opengl function */
      glTexEnvi(GL_TEXTURE_ENV, e, get_gl_enum(L, 2));

   else
      luaL_error(L, "incorrect argument to function 'gl.TexEnv'");

   return 0;
}

/*TexGen (coord, pname, param) -> none
  TexGen (coord, pname, paramsArray) -> none*/
int static gl_tex_gen(lua_State *L)
{
   GLenum e1, e2;
   GLdouble *param;

   /* test arguments type */
   if(!( lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.TexGen'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.TexGen'");

   if(lua_istable(L, 3))
   {
      get_arrayd(L, 3, &param);

      /* call opengl function */
      glTexGendv(e1, e2, (GLdouble *)param);

      free(param);
   }
   else if(lua_isstring(L, 3))
      /* call opengl function */
      glTexGeni(e1, e2, get_gl_enum(L, 3));

   else
      luaL_error(L, "incorrect argument to function 'gl.TexGen'");
   return 0;
}

/*TexImage(level, internalformat, format, pixels) -> none*/
// static int gl_tex_image(lua_State *L)
// {
//    GLenum e;
//    GLfloat *pixels;
//    GLsizei width, height;
//    int iformat;
//
//    /* test arguments type */
//    if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
//          lua_isstring(L, 3) && lua_istable(L, 4) ))
//       luaL_error(L, "incorrect argument to function 'gl.TexImage'");
//
//    e = get_gl_enum(L, 3);
//
//    /* test argument */
//    if(e == ENUM_ERROR)
//       luaL_error(L, "incorrect string argument to function 'gl.TexImage'");
//
//    iformat = (int)lua_tonumber(L, 2);
//
//    if((height = get_array2f(L, 4, &pixels, &width)) != -1)
//    {
//       glTexImage2D(GL_TEXTURE_2D, (GLint)lua_tonumber(L, 1),
//                    iformat, width/iformat, height, 0, e, GL_FLOAT, pixels);
//       return 0;
//    }
//    else
//    {
//       width = get_arrayf(L, 4, &pixels);
//       glTexImage1D(GL_TEXTURE_1D, (GLint)lua_tonumber(L, 1),
//                    iformat, width/iformat, 0, e, GL_FLOAT, pixels);
//       return 0;
//    }
// }
//
/*TexSubImage (level, format, pixels, xoffset) -> none
  TexSubImage (level, format, pixels, xoffset, yoffset) -> none*/
static int gl_tex_sub_image(lua_State *L)
{
   GLenum format;
   GLfloat *pixels;
   GLsizei width, height;
   int size = 1;

   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isstring(L, 2) &&
         lua_istable(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.TexSubImage'");

   format = get_gl_enum(L, 2);
   switch(format)
   {
      case GL_COLOR_INDEX:
      case GL_RED:
      case GL_GREEN:
      case GL_BLUE:
      case GL_ALPHA:
      case GL_LUMINANCE:
         size = 1;
         break;

      case GL_LUMINANCE_ALPHA:
         size = 2;
         break;

      case GL_RGB:
      case GL_BGR_EXT:
         size = 3;
         break;

      case GL_RGBA:
      case GL_BGRA_EXT:
         size = 4;
         break;
   }

   /* test argument */
   if(format == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.TexSubImage'");

   if((height = get_array2f(L, 3, &pixels, &width)) != -1)
   {
      glTexSubImage2D(GL_TEXTURE_2D, (GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 4),
                      (GLint)lua_tonumber(L, 5), width/size, height, format, GL_FLOAT, pixels);
      return 0;
   }
   else
   {
      width = get_arrayf(L, 3, &pixels);
      glTexSubImage1D(GL_TEXTURE_1D, (GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 4),
                      width/size, format, GL_FLOAT, pixels);
      return 0;
   }
}

/*TexParameter (target, pname, param) -> none
  TexParameter (target, pname, paramsArray) -> none*/
static int gl_tex_parameter(lua_State *L)
{
   GLenum e1, e2;
   GLfloat *param;

   /* test arguments type */
   if(! (lua_isstring(L, 1) && lua_isstring(L, 2) ))
      luaL_error(L, "incorrect argument to function 'gl.TexParameter'");

   /* get string parameters */
   e1 = get_gl_enum(L, 1);
   e2 = get_gl_enum(L, 2);

   /* test argument */
   if(e1 == ENUM_ERROR || e2 == ENUM_ERROR)
      luaL_error(L, "incorrect string argument to function 'gl.TexParameter'");

   if(lua_istable(L, 3))
   {
      get_arrayf(L, 3, &param);

      /* call opengl function */
      glTexParameterfv(e1, e2, (GLfloat *)param);

      free(param);
   }
   else if(lua_isnumber(L, 3))
   {
      /* call opengl function */
      glTexParameterf(e1, e2, (GLfloat)lua_tonumber(L, 3));
   }
   else if(lua_isstring(L, 3))
   {
      /* call opengl function */
      glTexParameteri(e1, e2, get_gl_enum(L, 3));
   }
   else
      luaL_error(L, "incorrect argument to function 'gl.TexParameter'");

   return 0;
}

/*Translate (x, y, z) -> none*/
static int gl_translate(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) ))
      luaL_error(L, "incorrect argument to function 'gl.Translate'");

   /* call opengl function */
   glTranslated((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2),
                (GLdouble)lua_tonumber(L, 3));

   return 0;
}

/*Vertex (x, y, [z, w]) -> none
  Vertex (v) -> none*/
static int gl_vertex(lua_State *L)
{
   int index;
   int num_args = lua_gettop(L);

   GLdouble *v;

   /* if have there's no arguments show an error message */
   if(num_args == 0)
      luaL_error(L, "incorrect argument to function 'gl.Vertex'");

   /* test argument type */
   if(lua_istable(L, 1))
      num_args = get_arrayd(L, 1, &v);

   else
   {
      /* test number of arguments */
      if(num_args < 2)
         luaL_error(L, "incorrect argument to function 'gl.Vertex'");

      v = (GLdouble *)malloc(num_args * sizeof(GLdouble));

      /* get arguments */
      for(index = 0; index < num_args; index++)
      {
         /* test arguments type */
         if(!lua_isnumber(L, index + 1))
            luaL_error(L, "incorrect argument to function 'gl.Vertex'");

         /* get argument */
         v[index] = (GLdouble)lua_tonumber(L, index + 1);
      }
   }

   /* if more then 4 arguments, ignore the others */
   if(num_args > 4)
      num_args = 4;

   /* call openGL functions */
   switch(num_args)
   {
      case 2:  glVertex2dv((GLdouble *)v);  break;
      case 3:  glVertex3dv((GLdouble *)v);  break;
      case 4:  glVertex4dv((GLdouble *)v);  break;
   }

   free(v);

   return 0;
}

/*VertexPointer (vertexArray) -> none*/
static int gl_vertex_pointer(lua_State *L)
{
   GLint size;
   static GLdouble *array = 0;

   if(array)
      free(array);

   /* test arguments type */
   if(!lua_istable(L, 1))
      luaL_error(L, "incorrect argument to function 'gl.VertexPointer'");

   if(lua_isnumber(L, 2))
   {
      size = (GLint)lua_tonumber(L, 2);
      get_arrayd(L, 1, &array);
   }
   else if(get_array2d(L, 1, &array, &size) == -1)
   {
      luaL_error(L, "incorrect argument to function 'gl.VertexPointer'");
      return 0;
   }

   /* call opengl function */
   glVertexPointer(size, GL_DOUBLE, 0, array);

   return 0;
}

/*Viewport (x, y, width, height) -> none*/
static int gl_viewport(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.Viewport'");

   /* call openGL function */
   glViewport((GLint)lua_tonumber(L, 1), (GLint)lua_tonumber(L, 2),
              (GLsizei)lua_tonumber(L, 3), (GLsizei)lua_tonumber(L, 4));

   return 0;
}

/*Perspective ( fovy, aspect, zNear, zFar ) -> none*/
static int gl_perspective(lua_State *L)
{
   /* test arguments type */
   if(!( lua_isnumber(L, 1) && lua_isnumber(L, 2) &&
         lua_isnumber(L, 3) && lua_isnumber(L, 4) ))
      luaL_error(L, "incorrect argument to function 'gl.Perspective'");

   GLdouble fovy   = (GLdouble)lua_tonumber(L, 1);
   GLdouble aspect = (GLdouble)lua_tonumber(L, 2);
   GLdouble zNear  = (GLdouble)lua_tonumber(L, 3);
   GLdouble zFar   = (GLdouble)lua_tonumber(L, 4);

   gluPerspective(fovy,aspect,zNear,zFar);
   /*
   GLdouble xmin, xmax, ymin, ymax;
   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
   */
   return 0;
}

static const luaL_reg gllib[] = {
  {"Accum", gl_accum},
  {"AlphaFunc", gl_alpha_func},
  {"AreTexturesResident", gl_are_textures_resident},
  {"ArrayElement", gl_array_element},
  {"Begin", gl_begin},
  {"BindTexture", gl_bind_texture},
  {"Bitmap", gl_bitmap},
  {"BlendFunc", gl_blend_func},
  {"CallList", gl_call_list},
  {"CallLists", gl_call_lists},
  {"Clear", gl_clear},
  {"ClearAccum", gl_clear_accum},
  {"ClearColor", gl_clear_color},
  {"ClearDepth", gl_clear_depth},
  {"ClearIndex", gl_clear_index},
  {"ClearStencil", gl_clear_stencil},
  {"ClipPlane", gl_clip_plane},
  {"Color", gl_color},
  {"ColorMask", gl_color_mask},
  {"ColorMaterial", gl_color_material},
  {"ColorPointer", gl_color_pointer},
  {"CopyPixels", gl_copy_pixels},
  {"CopyTexImage", gl_copy_tex_image},
  {"CopyTexSubImage", gl_copy_tex_sub_image},
  {"CullFace",gl_cull_face},
  {"DeleteLists",gl_delete_lists},
  {"DeleteTextures",gl_delete_textures},
  {"DepthFunc",gl_depth_func},
  {"DepthMask",gl_depth_mask},
  {"DepthRange",gl_depth_range},
  {"Disable",gl_disable},
  {"DisableClientState",gl_disable_client_state},
  {"DrawArrays",gl_draw_arrays},
  {"DrawBuffer",gl_draw_buffer},
  {"DrawElements", gl_draw_elements},
//  {"DrawPixels", gl_draw_pixels},
  {"EdgeFlag", gl_edge_flag},
  {"EdgeFlagPointer", gl_edge_flag_pointer},
  {"Enable", gl_enable},
  {"EnableClientState", gl_enable_client_state},
  {"End", gl_end},
  {"EndList", gl_end_list},
  {"EvalCoord", gl_eval_coord},
  {"EvalMesh", gl_eval_mesh},
  {"EvalPoint", gl_eval_point},
  {"FeedbackBuffer", gl_feedback_buffer},
  {"Finish", gl_finish},
  {"Flush", gl_flush},
  {"Fog", gl_fog},
  {"FrontFace", gl_front_face},
  {"Frustum", gl_frustum},
  {"GenLists", gl_gen_lists},
  {"GenTextures", gl_gen_textures},
  {"Get", gl_get},
  {"GetArray", gl_get_array},
  {"GetConst", gl_get_const},
  {"GetClipPlane", gl_get_clip_plane},
  {"GetError", gl_get_error},
  {"GetLight", gl_get_light},
  {"GetMap", gl_get_map},
  {"GetMaterial", gl_get_material},
  {"GetPixelMap", gl_get_pixel_map},
  {"GetPointer", gl_get_pointer},
  {"GetPolygonStipple", gl_get_polygon_stipple},
  {"GetString", gl_get_string},
  {"GetTexEnv", gl_get_tex_env},
  {"GetTexGen", gl_get_tex_gen},
  {"GetTexImage", gl_get_tex_image},
  {"GetTexLevelParameter", gl_get_tex_level_parameter},
  {"GetTexParameter", gl_get_tex_parameter},
  {"Hint", gl_hint},
  {"Index", gl_index},
  {"IndexMask", gl_index_mask},
  {"IndexPointer", gl_index_pointer},
  {"InitNames", gl_init_names},
  {"IsEnabled", gl_is_enabled},
  {"IsList", gl_is_list},
  {"IsTexture", gl_is_texture},
  {"Light", gl_light},
  {"LightModel", gl_light_model},
  {"LineStipple", gl_line_stipple},
  {"LineWidth", gl_line_width},
  {"ListBase", gl_list_base},
  {"LoadIdentity", gl_load_identity},
  {"LoadMatrix", gl_load_matrix},
  {"LoadName", gl_load_name},
  {"LogicOp", gl_logic_op},
  {"Map", gl_map},
  {"MapGrid", gl_map_grid},
  {"Material", gl_material},
  {"MatrixMode", gl_matrix_mode},
  {"MultMatrix", gl_mult_matrix},
  {"NewList", gl_new_list},
  {"Normal", gl_normal},
  {"NormalPointer", gl_normal_pointer},
  {"Ortho", gl_ortho},
  {"Ortho2D", gl_ortho2D},
  {"PassThrough", gl_pass_through},
  {"Perspective", gl_perspective},
  {"PixelMap", gl_pixel_map},
  {"PixelStore", gl_pixel_store},
  {"PixelTransfer", gl_pixel_transfer},
  {"PixelZoom", gl_pixel_zoom},
  {"PointSize", gl_point_size},
  {"PolygonMode", gl_polygon_mode},
  {"PolygonOffset", gl_polygon_offset},
  {"PolygonStipple", gl_polygon_stipple},
  {"PopAttrib", gl_pop_attrib},
  {"PopClientAttrib", gl_pop_client_attrib},
  {"PopMatrix", gl_pop_matrix},
  {"PopName", gl_pop_name},
  {"PrioritizeTextures", gl_prioritize_textures},
  {"PushAttrib", gl_push_attrib},
  {"PushClientAttrib", gl_push_client_attrib},
  {"PushMatrix", gl_push_matrix},
  {"PushName", gl_push_name},
  {"RasterPos", gl_raster_pos},
  {"ReadBuffer", gl_read_buffer},
  {"ReadPixels", gl_read_pixels},
  {"Rect", gl_rect},
  {"RenderMode", gl_render_mode},
  {"Rotate", gl_rotate},
  {"Scale", gl_scale},
  {"Scissor", gl_scissor},
  {"SelectBuffer", gl_select_buffer},
  {"ShadeModel", gl_shade_model},
  {"StencilFunc", gl_stencil_func},
  {"StencilMask", gl_stencil_mask},
  {"StencilOp", gl_stencil_op},
  {"TexCoord", gl_tex_coord},
  {"TexCoordPointer", gl_tex_coord_pointer},
  {"TexEnv", gl_tex_env},
  {"TexGen", gl_tex_gen},
//  {"TexImage", gl_tex_image},
  {"TexSubImage", gl_tex_sub_image},
  {"TexParameter", gl_tex_parameter},
  {"Translate", gl_translate},
  {"Vertex", gl_vertex},
  {"VertexPointer", gl_vertex_pointer},
  {"Viewport", gl_viewport},
  {NULL, NULL}
};

LUAGL_API int luaopen_gl_vendor(lua_State *L) {
  //luaL_openlib(L, "gl", gllib, 0);
  //
  luaL_register(L, "gl", gllib);
  return 1;
}
