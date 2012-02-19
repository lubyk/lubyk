/*************************************************
*  LuaGL - an OpenGL binding for Lua
*  2003-2004(c) Fabio Guerra, Cleyde Marlyse
*  http://luagl.sourceforge.net
*-------------------------------------------------
*  Description: This file implements the OpenGL
*               binding for Lua 5
*-------------------------------------------------
* Mantained by Antonio Scuri since 2009
*-------------------------------------------------
*  See Copyright Notice in LuaGL.h
*************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif
#if defined (__APPLE__) || defined (OSX)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <lua.h>
#include <lauxlib.h>

#include "LuaGL.h"
#include "luagl_util.h"


#define LUAGL_VERSION "1.6"


static const luaglConst luagl_const[] = {
  { "VERSION_1_1"                     , GL_VERSION_1_1                    },
#ifdef GL_VERSION_1_2
  { "VERSION_1_2"                     , GL_VERSION_1_2                    },
#endif
#ifdef GL_VERSION_1_3
  { "VERSION_1_3"                     , GL_VERSION_1_3                    },
#endif
  { "ACCUM"                           , GL_ACCUM                          },
  { "LOAD"                            , GL_LOAD                           },
  { "RETURN"                          , GL_RETURN                         },
  { "MULT"                            , GL_MULT                           },
  { "ADD"                             , GL_ADD                            },
  { "NEVER"                           , GL_NEVER                          },
  { "LESS"                            , GL_LESS                           },
  { "EQUAL"                           , GL_EQUAL                          },
  { "LEQUAL"                          , GL_LEQUAL                         },
  { "GREATER"                         , GL_GREATER                        },
  { "NOTEQUAL"                        , GL_NOTEQUAL                       },
  { "GEQUAL"                          , GL_GEQUAL                         },
  { "ALWAYS"                          , GL_ALWAYS                         },
  { "POINTS"                          , GL_POINTS                         },
  { "LINES"                           , GL_LINES                          },
  { "LINE_LOOP"                       , GL_LINE_LOOP                      },
  { "LINE_STRIP"                      , GL_LINE_STRIP                     },
  { "TRIANGLES"                       , GL_TRIANGLES                      },
  { "TRIANGLE_STRIP"                  , GL_TRIANGLE_STRIP                 },
  { "TRIANGLE_FAN"                    , GL_TRIANGLE_FAN                   },
  { "QUADS"                           , GL_QUADS                          },
  { "QUAD_STRIP"                      , GL_QUAD_STRIP                     },
  { "POLYGON"                         , GL_POLYGON                        },
  { "ZERO"                            , GL_ZERO                           },
  { "ONE"                             , GL_ONE                            },
  { "SRC_COLOR"                       , GL_SRC_COLOR                      },
  { "ONE_MINUS_SRC_COLOR"             , GL_ONE_MINUS_SRC_COLOR            },
  { "SRC_ALPHA"                       , GL_SRC_ALPHA                      },
  { "ONE_MINUS_SRC_ALPHA"             , GL_ONE_MINUS_SRC_ALPHA            },
  { "DST_ALPHA"                       , GL_DST_ALPHA                      },
  { "ONE_MINUS_DST_ALPHA"             , GL_ONE_MINUS_DST_ALPHA            },
  { "DST_COLOR"                       , GL_DST_COLOR                      },
  { "ONE_MINUS_DST_COLOR"             , GL_ONE_MINUS_DST_COLOR            },
  { "SRC_ALPHA_SATURATE"              , GL_SRC_ALPHA_SATURATE             },
  { "TRUE"                            , GL_TRUE                           },
  { "FALSE"                           , GL_FALSE                          },
  { "CLIP_PLANE0"                     , GL_CLIP_PLANE0                    },
  { "CLIP_PLANE1"                     , GL_CLIP_PLANE1                    },
  { "CLIP_PLANE2"                     , GL_CLIP_PLANE2                    },
  { "CLIP_PLANE3"                     , GL_CLIP_PLANE3                    },
  { "CLIP_PLANE4"                     , GL_CLIP_PLANE4                    },
  { "CLIP_PLANE5"                     , GL_CLIP_PLANE5                    },
  { "BYTE"                            , GL_BYTE                           },
  { "UNSIGNED_BYTE"                   , GL_UNSIGNED_BYTE                  },
  { "SHORT"                           , GL_SHORT                          },
  { "UNSIGNED_SHORT"                  , GL_UNSIGNED_SHORT                 },
  { "INT"                             , GL_INT                            },
  { "UNSIGNED_INT"                    , GL_UNSIGNED_INT                   },
  { "FLOAT"                           , GL_FLOAT                          },
  { "2_BYTES"                         , GL_2_BYTES                        },
  { "3_BYTES"                         , GL_3_BYTES                        },
  { "4_BYTES"                         , GL_4_BYTES                        },
  { "DOUBLE"                          , GL_DOUBLE                         },
  { "NONE"                            , GL_NONE                           },
  { "FRONT_LEFT"                      , GL_FRONT_LEFT                     },
  { "FRONT_RIGHT"                     , GL_FRONT_RIGHT                    },
  { "BACK_LEFT"                       , GL_BACK_LEFT                      },
  { "BACK_RIGHT"                      , GL_BACK_RIGHT                     },
  { "FRONT"                           , GL_FRONT                          },
  { "BACK"                            , GL_BACK                           },
  { "LEFT"                            , GL_LEFT                           },
  { "RIGHT"                           , GL_RIGHT                          },
  { "FRONT_AND_BACK"                  , GL_FRONT_AND_BACK                 },
  { "AUX0"                            , GL_AUX0                           },
  { "AUX1"                            , GL_AUX1                           },
  { "AUX2"                            , GL_AUX2                           },
  { "AUX3"                            , GL_AUX3                           },
  { "NO_ERROR"                        , GL_NO_ERROR                       },
  { "INVALID_ENUM"                    , GL_INVALID_ENUM                   },
  { "INVALID_VALUE"                   , GL_INVALID_VALUE                  },
  { "INVALID_OPERATION"               , GL_INVALID_OPERATION              },
  { "STACK_OVERFLOW"                  , GL_STACK_OVERFLOW                 },
  { "STACK_UNDERFLOW"                 , GL_STACK_UNDERFLOW                },
  { "OUT_OF_MEMORY"                   , GL_OUT_OF_MEMORY                  },
  { "2D"                              , GL_2D                             },
  { "3D"                              , GL_3D                             },
  { "3D_COLOR"                        , GL_3D_COLOR                       },
  { "3D_COLOR_TEXTURE"                , GL_3D_COLOR_TEXTURE               },
  { "4D_COLOR_TEXTURE"                , GL_4D_COLOR_TEXTURE               },
  { "PASS_THROUGH_TOKEN"              , GL_PASS_THROUGH_TOKEN             },
  { "POINT_TOKEN"                     , GL_POINT_TOKEN                    },
  { "LINE_TOKEN"                      , GL_LINE_TOKEN                     },
  { "POLYGON_TOKEN"                   , GL_POLYGON_TOKEN                  },
  { "BITMAP_TOKEN"                    , GL_BITMAP_TOKEN                   },
  { "DRAW_PIXEL_TOKEN"                , GL_DRAW_PIXEL_TOKEN               },
  { "COPY_PIXEL_TOKEN"                , GL_COPY_PIXEL_TOKEN               },
  { "LINE_RESET_TOKEN"                , GL_LINE_RESET_TOKEN               },
  { "EXP"                             , GL_EXP                            },
  { "EXP2"                            , GL_EXP2                           },
  { "CW"                              , GL_CW                             },
  { "CCW"                             , GL_CCW                            },
  { "COEFF"                           , GL_COEFF                          },
  { "ORDER"                           , GL_ORDER                          },
  { "DOMAIN"                          , GL_DOMAIN                         },
  { "CURRENT_COLOR"                   , GL_CURRENT_COLOR                  },
  { "CURRENT_INDEX"                   , GL_CURRENT_INDEX                  },
  { "CURRENT_NORMAL"                  , GL_CURRENT_NORMAL                 },
  { "CURRENT_TEXTURE_COORDS"          , GL_CURRENT_TEXTURE_COORDS         },
  { "CURRENT_RASTER_COLOR"            , GL_CURRENT_RASTER_COLOR           },
  { "CURRENT_RASTER_INDEX"            , GL_CURRENT_RASTER_INDEX           },
  { "CURRENT_RASTER_TEXTURE_COORDS"   , GL_CURRENT_RASTER_TEXTURE_COORDS  },
  { "CURRENT_RASTER_POSITION"         , GL_CURRENT_RASTER_POSITION        },
  { "CURRENT_RASTER_POSITION_VALID"   , GL_CURRENT_RASTER_POSITION_VALID  },
  { "CURRENT_RASTER_DISTANCE"         , GL_CURRENT_RASTER_DISTANCE        },
  { "POINT_SMOOTH"                    , GL_POINT_SMOOTH                   },
  { "POINT_SIZE"                      , GL_POINT_SIZE                     },
  { "POINT_SIZE_RANGE"                , GL_POINT_SIZE_RANGE               },
  { "POINT_SIZE_GRANULARITY"          , GL_POINT_SIZE_GRANULARITY         },
  { "LINE_SMOOTH"                     , GL_LINE_SMOOTH                    },
  { "LINE_WIDTH"                      , GL_LINE_WIDTH                     },
  { "LINE_WIDTH_RANGE"                , GL_LINE_WIDTH_RANGE               },
  { "LINE_WIDTH_GRANULARITY"          , GL_LINE_WIDTH_GRANULARITY         },
  { "LINE_STIPPLE"                    , GL_LINE_STIPPLE                   },
  { "LINE_STIPPLE_PATTERN"            , GL_LINE_STIPPLE_PATTERN           },
  { "LINE_STIPPLE_REPEAT"             , GL_LINE_STIPPLE_REPEAT            },
  { "LIST_MODE"                       , GL_LIST_MODE                      },
  { "MAX_LIST_NESTING"                , GL_MAX_LIST_NESTING               },
  { "LIST_BASE"                       , GL_LIST_BASE                      },
  { "LIST_INDEX"                      , GL_LIST_INDEX                     },
  { "POLYGON_MODE"                    , GL_POLYGON_MODE                   },
  { "POLYGON_SMOOTH"                  , GL_POLYGON_SMOOTH                 },
  { "POLYGON_STIPPLE"                 , GL_POLYGON_STIPPLE                },
  { "EDGE_FLAG"                       , GL_EDGE_FLAG                      },
  { "CULL_FACE"                       , GL_CULL_FACE                      },
  { "CULL_FACE_MODE"                  , GL_CULL_FACE_MODE                 },
  { "FRONT_FACE"                      , GL_FRONT_FACE                     },
  { "LIGHTING"                        , GL_LIGHTING                       },
  { "LIGHT_MODEL_LOCAL_VIEWER"        , GL_LIGHT_MODEL_LOCAL_VIEWER       },
  { "LIGHT_MODEL_TWO_SIDE"            , GL_LIGHT_MODEL_TWO_SIDE           },
  { "LIGHT_MODEL_AMBIENT"             , GL_LIGHT_MODEL_AMBIENT            },
  { "SHADE_MODEL"                     , GL_SHADE_MODEL                    },
  { "COLOR_MATERIAL_FACE"             , GL_COLOR_MATERIAL_FACE            },
  { "COLOR_MATERIAL_PARAMETER"        , GL_COLOR_MATERIAL_PARAMETER       },
  { "COLOR_MATERIAL"                  , GL_COLOR_MATERIAL                 },
  { "FOG"                             , GL_FOG                            },
  { "FOG_INDEX"                       , GL_FOG_INDEX                      },
  { "FOG_DENSITY"                     , GL_FOG_DENSITY                    },
  { "FOG_START"                       , GL_FOG_START                      },
  { "FOG_END"                         , GL_FOG_END                        },
  { "FOG_MODE"                        , GL_FOG_MODE                       },
  { "FOG_COLOR"                       , GL_FOG_COLOR                      },
  { "DEPTH_RANGE"                     , GL_DEPTH_RANGE                    },
  { "DEPTH_TEST"                      , GL_DEPTH_TEST                     },
  { "DEPTH_WRITEMASK"                 , GL_DEPTH_WRITEMASK                },
  { "DEPTH_CLEAR_VALUE"               , GL_DEPTH_CLEAR_VALUE              },
  { "DEPTH_FUNC"                      , GL_DEPTH_FUNC                     },
  { "ACCUM_CLEAR_VALUE"               , GL_ACCUM_CLEAR_VALUE              },
  { "STENCIL_TEST"                    , GL_STENCIL_TEST                   },
  { "STENCIL_CLEAR_VALUE"             , GL_STENCIL_CLEAR_VALUE            },
  { "STENCIL_FUNC"                    , GL_STENCIL_FUNC                   },
  { "STENCIL_VALUE_MASK"              , GL_STENCIL_VALUE_MASK             },
  { "STENCIL_FAIL"                    , GL_STENCIL_FAIL                   },
  { "STENCIL_PASS_DEPTH_FAIL"         , GL_STENCIL_PASS_DEPTH_FAIL        },
  { "STENCIL_PASS_DEPTH_PASS"         , GL_STENCIL_PASS_DEPTH_PASS        },
  { "STENCIL_REF"                     , GL_STENCIL_REF                    },
  { "STENCIL_WRITEMASK"               , GL_STENCIL_WRITEMASK              },
  { "MATRIX_MODE"                     , GL_MATRIX_MODE                    },
  { "NORMALIZE"                       , GL_NORMALIZE                      },
  { "VIEWPORT"                        , GL_VIEWPORT                       },
  { "MODELVIEW_STACK_DEPTH"           , GL_MODELVIEW_STACK_DEPTH          },
  { "PROJECTION_STACK_DEPTH"          , GL_PROJECTION_STACK_DEPTH         },
  { "TEXTURE_STACK_DEPTH"             , GL_TEXTURE_STACK_DEPTH            },
  { "MODELVIEW_MATRIX"                , GL_MODELVIEW_MATRIX               },
  { "PROJECTION_MATRIX"               , GL_PROJECTION_MATRIX              },
  { "TEXTURE_MATRIX"                  , GL_TEXTURE_MATRIX                 },
  { "ATTRIB_STACK_DEPTH"              , GL_ATTRIB_STACK_DEPTH             },
  { "CLIENT_ATTRIB_STACK_DEPTH"       , GL_CLIENT_ATTRIB_STACK_DEPTH      },
  { "ALPHA_TEST"                      , GL_ALPHA_TEST                     },
  { "ALPHA_TEST_FUNC"                 , GL_ALPHA_TEST_FUNC                },
  { "ALPHA_TEST_REF"                  , GL_ALPHA_TEST_REF                 },
  { "DITHER"                          , GL_DITHER                         },
  { "BLEND_DST"                       , GL_BLEND_DST                      },
  { "BLEND_SRC"                       , GL_BLEND_SRC                      },
  { "BLEND"                           , GL_BLEND                          },
  { "LOGIC_OP_MODE"                   , GL_LOGIC_OP_MODE                  },
  { "LOGIC_OP"                        , GL_LOGIC_OP                       },
  { "INDEX_LOGIC_OP"                  , GL_INDEX_LOGIC_OP                 },
  { "COLOR_LOGIC_OP"                  , GL_COLOR_LOGIC_OP                 },
  { "AUX_BUFFERS"                     , GL_AUX_BUFFERS                    },
  { "DRAW_BUFFER"                     , GL_DRAW_BUFFER                    },
  { "READ_BUFFER"                     , GL_READ_BUFFER                    },
  { "SCISSOR_BOX"                     , GL_SCISSOR_BOX                    },
  { "SCISSOR_TEST"                    , GL_SCISSOR_TEST                   },
  { "INDEX_CLEAR_VALUE"               , GL_INDEX_CLEAR_VALUE              },
  { "INDEX_WRITEMASK"                 , GL_INDEX_WRITEMASK                },
  { "COLOR_CLEAR_VALUE"               , GL_COLOR_CLEAR_VALUE              },
  { "COLOR_WRITEMASK"                 , GL_COLOR_WRITEMASK                },
  { "INDEX_MODE"                      , GL_INDEX_MODE                     },
  { "RGBA_MODE"                       , GL_RGBA_MODE                      },
  { "DOUBLEBUFFER"                    , GL_DOUBLEBUFFER                   },
  { "STEREO"                          , GL_STEREO                         },
  { "RENDER_MODE"                     , GL_RENDER_MODE                    },
  { "PERSPECTIVE_CORRECTION_HINT"     , GL_PERSPECTIVE_CORRECTION_HINT    },
  { "POINT_SMOOTH_HINT"               , GL_POINT_SMOOTH_HINT              },
  { "LINE_SMOOTH_HINT"                , GL_LINE_SMOOTH_HINT               },
  { "POLYGON_SMOOTH_HINT"             , GL_POLYGON_SMOOTH_HINT            },
  { "FOG_HINT"                        , GL_FOG_HINT                       },
  { "TEXTURE_GEN_S"                   , GL_TEXTURE_GEN_S                  },
  { "TEXTURE_GEN_T"                   , GL_TEXTURE_GEN_T                  },
  { "TEXTURE_GEN_R"                   , GL_TEXTURE_GEN_R                  },
  { "TEXTURE_GEN_Q"                   , GL_TEXTURE_GEN_Q                  },
  { "PIXEL_MAP_I_TO_I"                , GL_PIXEL_MAP_I_TO_I               },
  { "PIXEL_MAP_S_TO_S"                , GL_PIXEL_MAP_S_TO_S               },
  { "PIXEL_MAP_I_TO_R"                , GL_PIXEL_MAP_I_TO_R               },
  { "PIXEL_MAP_I_TO_G"                , GL_PIXEL_MAP_I_TO_G               },
  { "PIXEL_MAP_I_TO_B"                , GL_PIXEL_MAP_I_TO_B               },
  { "PIXEL_MAP_I_TO_A"                , GL_PIXEL_MAP_I_TO_A               },
  { "PIXEL_MAP_R_TO_R"                , GL_PIXEL_MAP_R_TO_R               },
  { "PIXEL_MAP_G_TO_G"                , GL_PIXEL_MAP_G_TO_G               },
  { "PIXEL_MAP_B_TO_B"                , GL_PIXEL_MAP_B_TO_B               },
  { "PIXEL_MAP_A_TO_A"                , GL_PIXEL_MAP_A_TO_A               },
  { "PIXEL_MAP_I_TO_I_SIZE"           , GL_PIXEL_MAP_I_TO_I_SIZE          },
  { "PIXEL_MAP_S_TO_S_SIZE"           , GL_PIXEL_MAP_S_TO_S_SIZE          },
  { "PIXEL_MAP_I_TO_R_SIZE"           , GL_PIXEL_MAP_I_TO_R_SIZE          },
  { "PIXEL_MAP_I_TO_G_SIZE"           , GL_PIXEL_MAP_I_TO_G_SIZE          },
  { "PIXEL_MAP_I_TO_B_SIZE"           , GL_PIXEL_MAP_I_TO_B_SIZE          },
  { "PIXEL_MAP_I_TO_A_SIZE"           , GL_PIXEL_MAP_I_TO_A_SIZE          },
  { "PIXEL_MAP_R_TO_R_SIZE"           , GL_PIXEL_MAP_R_TO_R_SIZE          },
  { "PIXEL_MAP_G_TO_G_SIZE"           , GL_PIXEL_MAP_G_TO_G_SIZE          },
  { "PIXEL_MAP_B_TO_B_SIZE"           , GL_PIXEL_MAP_B_TO_B_SIZE          },
  { "PIXEL_MAP_A_TO_A_SIZE"           , GL_PIXEL_MAP_A_TO_A_SIZE          },
  { "UNPACK_SWAP_BYTES"               , GL_UNPACK_SWAP_BYTES              },
  { "UNPACK_LSB_FIRST"                , GL_UNPACK_LSB_FIRST               },
  { "UNPACK_ROW_LENGTH"               , GL_UNPACK_ROW_LENGTH              },
  { "UNPACK_SKIP_ROWS"                , GL_UNPACK_SKIP_ROWS               },
  { "UNPACK_SKIP_PIXELS"              , GL_UNPACK_SKIP_PIXELS             },
  { "UNPACK_ALIGNMENT"                , GL_UNPACK_ALIGNMENT               },
  { "PACK_SWAP_BYTES"                 , GL_PACK_SWAP_BYTES                },
  { "PACK_LSB_FIRST"                  , GL_PACK_LSB_FIRST                 },
  { "PACK_ROW_LENGTH"                 , GL_PACK_ROW_LENGTH                },
  { "PACK_SKIP_ROWS"                  , GL_PACK_SKIP_ROWS                 },
  { "PACK_SKIP_PIXELS"                , GL_PACK_SKIP_PIXELS               },
  { "PACK_ALIGNMENT"                  , GL_PACK_ALIGNMENT                 },
  { "MAP_COLOR"                       , GL_MAP_COLOR                      },
  { "MAP_STENCIL"                     , GL_MAP_STENCIL                    },
  { "INDEX_SHIFT"                     , GL_INDEX_SHIFT                    },
  { "INDEX_OFFSET"                    , GL_INDEX_OFFSET                   },
  { "RED_SCALE"                       , GL_RED_SCALE                      },
  { "RED_BIAS"                        , GL_RED_BIAS                       },
  { "ZOOM_X"                          , GL_ZOOM_X                         },
  { "ZOOM_Y"                          , GL_ZOOM_Y                         },
  { "GREEN_SCALE"                     , GL_GREEN_SCALE                    },
  { "GREEN_BIAS"                      , GL_GREEN_BIAS                     },
  { "BLUE_SCALE"                      , GL_BLUE_SCALE                     },
  { "BLUE_BIAS"                       , GL_BLUE_BIAS                      },
  { "ALPHA_SCALE"                     , GL_ALPHA_SCALE                    },
  { "ALPHA_BIAS"                      , GL_ALPHA_BIAS                     },
  { "DEPTH_SCALE"                     , GL_DEPTH_SCALE                    },
  { "DEPTH_BIAS"                      , GL_DEPTH_BIAS                     },
  { "MAX_EVAL_ORDER"                  , GL_MAX_EVAL_ORDER                 },
  { "MAX_LIGHTS"                      , GL_MAX_LIGHTS                     },
  { "MAX_CLIP_PLANES"                 , GL_MAX_CLIP_PLANES                },
  { "MAX_TEXTURE_SIZE"                , GL_MAX_TEXTURE_SIZE               },
  { "MAX_PIXEL_MAP_TABLE"             , GL_MAX_PIXEL_MAP_TABLE            },
  { "MAX_ATTRIB_STACK_DEPTH"          , GL_MAX_ATTRIB_STACK_DEPTH         },
  { "MAX_MODELVIEW_STACK_DEPTH"       , GL_MAX_MODELVIEW_STACK_DEPTH      },
  { "MAX_NAME_STACK_DEPTH"            , GL_MAX_NAME_STACK_DEPTH           },
  { "MAX_PROJECTION_STACK_DEPTH"      , GL_MAX_PROJECTION_STACK_DEPTH     },
  { "MAX_TEXTURE_STACK_DEPTH"         , GL_MAX_TEXTURE_STACK_DEPTH        },
  { "MAX_VIEWPORT_DIMS"               , GL_MAX_VIEWPORT_DIMS              },
  { "MAX_CLIENT_ATTRIB_STACK_DEPTH"   , GL_MAX_CLIENT_ATTRIB_STACK_DEPTH  },
  { "SUBPIXEL_BITS"                   , GL_SUBPIXEL_BITS                  },
  { "INDEX_BITS"                      , GL_INDEX_BITS                     },
  { "RED_BITS"                        , GL_RED_BITS                       },
  { "GREEN_BITS"                      , GL_GREEN_BITS                     },
  { "BLUE_BITS"                       , GL_BLUE_BITS                      },
  { "ALPHA_BITS"                      , GL_ALPHA_BITS                     },
  { "DEPTH_BITS"                      , GL_DEPTH_BITS                     },
  { "STENCIL_BITS"                    , GL_STENCIL_BITS                   },
  { "ACCUM_RED_BITS"                  , GL_ACCUM_RED_BITS                 },
  { "ACCUM_GREEN_BITS"                , GL_ACCUM_GREEN_BITS               },
  { "ACCUM_BLUE_BITS"                 , GL_ACCUM_BLUE_BITS                },
  { "ACCUM_ALPHA_BITS"                , GL_ACCUM_ALPHA_BITS               },
  { "NAME_STACK_DEPTH"                , GL_NAME_STACK_DEPTH               },
  { "AUTO_NORMAL"                     , GL_AUTO_NORMAL                    },
  { "MAP1_COLOR_4"                    , GL_MAP1_COLOR_4                   },
  { "MAP1_INDEX"                      , GL_MAP1_INDEX                     },
  { "MAP1_NORMAL"                     , GL_MAP1_NORMAL                    },
  { "MAP1_TEXTURE_COORD_1"            , GL_MAP1_TEXTURE_COORD_1           },
  { "MAP1_TEXTURE_COORD_2"            , GL_MAP1_TEXTURE_COORD_2           },
  { "MAP1_TEXTURE_COORD_3"            , GL_MAP1_TEXTURE_COORD_3           },
  { "MAP1_TEXTURE_COORD_4"            , GL_MAP1_TEXTURE_COORD_4           },
  { "MAP1_VERTEX_3"                   , GL_MAP1_VERTEX_3                  },
  { "MAP1_VERTEX_4"                   , GL_MAP1_VERTEX_4                  },
  { "MAP2_COLOR_4"                    , GL_MAP2_COLOR_4                   },
  { "MAP2_INDEX"                      , GL_MAP2_INDEX                     },
  { "MAP2_NORMAL"                     , GL_MAP2_NORMAL                    },
  { "MAP2_TEXTURE_COORD_1"            , GL_MAP2_TEXTURE_COORD_1           },
  { "MAP2_TEXTURE_COORD_2"            , GL_MAP2_TEXTURE_COORD_2           },
  { "MAP2_TEXTURE_COORD_3"            , GL_MAP2_TEXTURE_COORD_3           },
  { "MAP2_TEXTURE_COORD_4"            , GL_MAP2_TEXTURE_COORD_4           },
  { "MAP2_VERTEX_3"                   , GL_MAP2_VERTEX_3                  },
  { "MAP2_VERTEX_4"                   , GL_MAP2_VERTEX_4                  },
  { "MAP1_GRID_DOMAIN"                , GL_MAP1_GRID_DOMAIN               },
  { "MAP1_GRID_SEGMENTS"              , GL_MAP1_GRID_SEGMENTS             },
  { "MAP2_GRID_DOMAIN"                , GL_MAP2_GRID_DOMAIN               },
  { "MAP2_GRID_SEGMENTS"              , GL_MAP2_GRID_SEGMENTS             },
  { "TEXTURE_1D"                      , GL_TEXTURE_1D                     },
  { "TEXTURE_2D"                      , GL_TEXTURE_2D                     },
  { "FEEDBACK_BUFFER_POINTER"         , GL_FEEDBACK_BUFFER_POINTER        },
  { "FEEDBACK_BUFFER_SIZE"            , GL_FEEDBACK_BUFFER_SIZE           },
  { "FEEDBACK_BUFFER_TYPE"            , GL_FEEDBACK_BUFFER_TYPE           },
  { "SELECTION_BUFFER_POINTER"        , GL_SELECTION_BUFFER_POINTER       },
  { "SELECTION_BUFFER_SIZE"           , GL_SELECTION_BUFFER_SIZE          },
  { "TEXTURE_WIDTH"                   , GL_TEXTURE_WIDTH                  },
  { "TEXTURE_HEIGHT"                  , GL_TEXTURE_HEIGHT                 },
  { "TEXTURE_COMPONENTS"              , GL_TEXTURE_COMPONENTS             },
  { "TEXTURE_INTERNAL_FORMAT"         , GL_TEXTURE_INTERNAL_FORMAT        },
  { "TEXTURE_BORDER_COLOR"            , GL_TEXTURE_BORDER_COLOR           },
  { "TEXTURE_BORDER"                  , GL_TEXTURE_BORDER                 },
  { "DONT_CARE"                       , GL_DONT_CARE                      },
  { "FASTEST"                         , GL_FASTEST                        },
  { "NICEST"                          , GL_NICEST                         },
  { "LIGHT0"                          , GL_LIGHT0                         },
  { "LIGHT1"                          , GL_LIGHT1                         },
  { "LIGHT2"                          , GL_LIGHT2                         },
  { "LIGHT3"                          , GL_LIGHT3                         },
  { "LIGHT4"                          , GL_LIGHT4                         },
  { "LIGHT5"                          , GL_LIGHT5                         },
  { "LIGHT6"                          , GL_LIGHT6                         },
  { "LIGHT7"                          , GL_LIGHT7                         },
  { "AMBIENT"                         , GL_AMBIENT                        },
  { "DIFFUSE"                         , GL_DIFFUSE                        },
  { "SPECULAR"                        , GL_SPECULAR                       },
  { "POSITION"                        , GL_POSITION                       },
  { "SPOT_DIRECTION"                  , GL_SPOT_DIRECTION                 },
  { "SPOT_EXPONENT"                   , GL_SPOT_EXPONENT                  },
  { "SPOT_CUTOFF"                     , GL_SPOT_CUTOFF                    },
  { "CONSTANT_ATTENUATION"            , GL_CONSTANT_ATTENUATION           },
  { "LINEAR_ATTENUATION"              , GL_LINEAR_ATTENUATION             },
  { "QUADRATIC_ATTENUATION"           , GL_QUADRATIC_ATTENUATION          },
  { "COMPILE"                         , GL_COMPILE                        },
  { "COMPILE_AND_EXECUTE"             , GL_COMPILE_AND_EXECUTE            },
  { "CLEAR"                           , GL_CLEAR                          },
  { "AND"                             , GL_AND                            },
  { "AND_REVERSE"                     , GL_AND_REVERSE                    },
  { "COPY"                            , GL_COPY                           },
  { "AND_INVERTED"                    , GL_AND_INVERTED                   },
  { "NOOP"                            , GL_NOOP                           },
  { "XOR"                             , GL_XOR                            },
  { "OR"                              , GL_OR                             },
  { "NOR"                             , GL_NOR                            },
  { "EQUIV"                           , GL_EQUIV                          },
  { "INVERT"                          , GL_INVERT                         },
  { "OR_REVERSE"                      , GL_OR_REVERSE                     },
  { "COPY_INVERTED"                   , GL_COPY_INVERTED                  },
  { "OR_INVERTED"                     , GL_OR_INVERTED                    },
  { "NAND"                            , GL_NAND                           },
  { "SET"                             , GL_SET                            },
  { "EMISSION"                        , GL_EMISSION                       },
  { "SHININESS"                       , GL_SHININESS                      },
  { "AMBIENT_AND_DIFFUSE"             , GL_AMBIENT_AND_DIFFUSE            },
  { "COLOR_INDEXES"                   , GL_COLOR_INDEXES                  },
  { "MODELVIEW"                       , GL_MODELVIEW                      },
  { "PROJECTION"                      , GL_PROJECTION                     },
  { "TEXTURE"                         , GL_TEXTURE                        },
  { "COLOR"                           , GL_COLOR                          },
  { "DEPTH"                           , GL_DEPTH                          },
  { "STENCIL"                         , GL_STENCIL                        },
  { "COLOR_INDEX"                     , GL_COLOR_INDEX                    },
  { "STENCIL_INDEX"                   , GL_STENCIL_INDEX                  },
  { "DEPTH_COMPONENT"                 , GL_DEPTH_COMPONENT                },
  { "RED"                             , GL_RED                            },
  { "GREEN"                           , GL_GREEN                          },
  { "BLUE"                            , GL_BLUE                           },
  { "ALPHA"                           , GL_ALPHA                          },
  { "RGB"                             , GL_RGB                            },
  { "RGBA"                            , GL_RGBA                           },
  { "LUMINANCE"                       , GL_LUMINANCE                      },
  { "LUMINANCE_ALPHA"                 , GL_LUMINANCE_ALPHA                },
  { "BITMAP"                          , GL_BITMAP                         },
  { "POINT"                           , GL_POINT                          },
  { "LINE"                            , GL_LINE                           },
  { "FILL"                            , GL_FILL                           },
  { "RENDER"                          , GL_RENDER                         },
  { "FEEDBACK"                        , GL_FEEDBACK                       },
  { "SELECT"                          , GL_SELECT                         },
  { "FLAT"                            , GL_FLAT                           },
  { "SMOOTH"                          , GL_SMOOTH                         },
  { "KEEP"                            , GL_KEEP                           },
  { "REPLACE"                         , GL_REPLACE                        },
  { "INCR"                            , GL_INCR                           },
  { "DECR"                            , GL_DECR                           },
  { "VENDOR"                          , GL_VENDOR                         },
  { "RENDERER"                        , GL_RENDERER                       },
  { "VERSION"                         , GL_VERSION                        },
  { "EXTENSIONS"                      , GL_EXTENSIONS                     },
  { "S"                               , GL_S                              },
  { "T"                               , GL_T                              },
  { "R"                               , GL_R                              },
  { "Q"                               , GL_Q                              },
  { "MODULATE"                        , GL_MODULATE                       },
  { "DECAL"                           , GL_DECAL                          },
  { "TEXTURE_ENV_MODE"                , GL_TEXTURE_ENV_MODE               },
  { "TEXTURE_ENV_COLOR"               , GL_TEXTURE_ENV_COLOR              },
  { "TEXTURE_ENV"                     , GL_TEXTURE_ENV                    },
  { "EYE_LINEAR"                      , GL_EYE_LINEAR                     },
  { "OBJECT_LINEAR"                   , GL_OBJECT_LINEAR                  },
  { "SPHERE_MAP"                      , GL_SPHERE_MAP                     },
  { "TEXTURE_GEN_MODE"                , GL_TEXTURE_GEN_MODE               },
  { "OBJECT_PLANE"                    , GL_OBJECT_PLANE                   },
  { "EYE_PLANE"                       , GL_EYE_PLANE                      },
  { "NEAREST"                         , GL_NEAREST                        },
  { "LINEAR"                          , GL_LINEAR                         },
  { "NEAREST_MIPMAP_NEAREST"          , GL_NEAREST_MIPMAP_NEAREST         },
  { "LINEAR_MIPMAP_NEAREST"           , GL_LINEAR_MIPMAP_NEAREST          },
  { "NEAREST_MIPMAP_LINEAR"           , GL_NEAREST_MIPMAP_LINEAR          },
  { "LINEAR_MIPMAP_LINEAR"            , GL_LINEAR_MIPMAP_LINEAR           },
  { "TEXTURE_MAG_FILTER"              , GL_TEXTURE_MAG_FILTER             },
  { "TEXTURE_MIN_FILTER"              , GL_TEXTURE_MIN_FILTER             },
  { "TEXTURE_WRAP_S"                  , GL_TEXTURE_WRAP_S                 },
  { "TEXTURE_WRAP_T"                  , GL_TEXTURE_WRAP_T                 },
  { "CLAMP"                           , GL_CLAMP                          },
  { "REPEAT"                          , GL_REPEAT                         },
#ifdef GL_MIRRORED_REPEAT
  { "MIRRORED_REPEAT"                 , GL_MIRRORED_REPEAT                },
#endif
#ifdef GL_VERSION_1_2
  { "CLAMP_TO_EDGE"                   , GL_CLAMP_TO_EDGE                  },
#endif
  { "POLYGON_OFFSET_FACTOR"           , GL_POLYGON_OFFSET_FACTOR          },
  { "POLYGON_OFFSET_UNITS"            , GL_POLYGON_OFFSET_UNITS           },
  { "POLYGON_OFFSET_POINT"            , GL_POLYGON_OFFSET_POINT           },
  { "POLYGON_OFFSET_LINE"             , GL_POLYGON_OFFSET_LINE            },
  { "POLYGON_OFFSET_FILL"             , GL_POLYGON_OFFSET_FILL            },
  { "ALPHA4"                          , GL_ALPHA4                         },
  { "ALPHA8"                          , GL_ALPHA8                         },
  { "ALPHA12"                         , GL_ALPHA12                        },
  { "ALPHA16"                         , GL_ALPHA16                        },
  { "LUMINANCE4"                      , GL_LUMINANCE4                     },
  { "LUMINANCE8"                      , GL_LUMINANCE8                     },
  { "LUMINANCE12"                     , GL_LUMINANCE12                    },
  { "LUMINANCE16"                     , GL_LUMINANCE16                    },
  { "LUMINANCE4_ALPHA4"               , GL_LUMINANCE4_ALPHA4              },
  { "LUMINANCE6_ALPHA2"               , GL_LUMINANCE6_ALPHA2              },
  { "LUMINANCE8_ALPHA8"               , GL_LUMINANCE8_ALPHA8              },
  { "LUMINANCE12_ALPHA4"              , GL_LUMINANCE12_ALPHA4             },
  { "LUMINANCE12_ALPHA12"             , GL_LUMINANCE12_ALPHA12            },
  { "LUMINANCE16_ALPHA16"             , GL_LUMINANCE16_ALPHA16            },
  { "INTENSITY"                       , GL_INTENSITY                      },
  { "INTENSITY4"                      , GL_INTENSITY4                     },
  { "INTENSITY8"                      , GL_INTENSITY8                     },
  { "INTENSITY12"                     , GL_INTENSITY12                    },
  { "INTENSITY16"                     , GL_INTENSITY16                    },
  { "R3_G3_B2"                        , GL_R3_G3_B2                       },
  { "RGB4"                            , GL_RGB4                           },
  { "RGB5"                            , GL_RGB5                           },
  { "RGB8"                            , GL_RGB8                           },
  { "RGB10"                           , GL_RGB10                          },
  { "RGB12"                           , GL_RGB12                          },
  { "RGB16"                           , GL_RGB16                          },
  { "RGBA2"                           , GL_RGBA2                          },
  { "RGBA4"                           , GL_RGBA4                          },
  { "RGB5_A1"                         , GL_RGB5_A1                        },
  { "RGBA8"                           , GL_RGBA8                          },
  { "RGB10_A2"                        , GL_RGB10_A2                       },
  { "RGBA12"                          , GL_RGBA12                         },
  { "RGBA16"                          , GL_RGBA16                         },
  { "TEXTURE_RED_SIZE"                , GL_TEXTURE_RED_SIZE               },
  { "TEXTURE_GREEN_SIZE"              , GL_TEXTURE_GREEN_SIZE             },
  { "TEXTURE_BLUE_SIZE"               , GL_TEXTURE_BLUE_SIZE              },
  { "TEXTURE_ALPHA_SIZE"              , GL_TEXTURE_ALPHA_SIZE             },
  { "TEXTURE_LUMINANCE_SIZE"          , GL_TEXTURE_LUMINANCE_SIZE         },
  { "TEXTURE_INTENSITY_SIZE"          , GL_TEXTURE_INTENSITY_SIZE         },
  { "PROXY_TEXTURE_1D"                , GL_PROXY_TEXTURE_1D               },
  { "PROXY_TEXTURE_2D"                , GL_PROXY_TEXTURE_2D               },
  { "TEXTURE_PRIORITY"                , GL_TEXTURE_PRIORITY               },
  { "TEXTURE_RESIDENT"                , GL_TEXTURE_RESIDENT               },
  { "TEXTURE_BINDING_1D"              , GL_TEXTURE_BINDING_1D             },
  { "TEXTURE_BINDING_2D"              , GL_TEXTURE_BINDING_2D             },
  { "VERTEX_ARRAY"                    , GL_VERTEX_ARRAY                   },
  { "NORMAL_ARRAY"                    , GL_NORMAL_ARRAY                   },
  { "COLOR_ARRAY"                     , GL_COLOR_ARRAY                    },
  { "INDEX_ARRAY"                     , GL_INDEX_ARRAY                    },
  { "TEXTURE_COORD_ARRAY"             , GL_TEXTURE_COORD_ARRAY            },
  { "EDGE_FLAG_ARRAY"                 , GL_EDGE_FLAG_ARRAY                },
  { "VERTEX_ARRAY_SIZE"               , GL_VERTEX_ARRAY_SIZE              },
  { "VERTEX_ARRAY_TYPE"               , GL_VERTEX_ARRAY_TYPE              },
  { "VERTEX_ARRAY_STRIDE"             , GL_VERTEX_ARRAY_STRIDE            },
  { "NORMAL_ARRAY_TYPE"               , GL_NORMAL_ARRAY_TYPE              },
  { "NORMAL_ARRAY_STRIDE"             , GL_NORMAL_ARRAY_STRIDE            },
  { "COLOR_ARRAY_SIZE"                , GL_COLOR_ARRAY_SIZE               },
  { "COLOR_ARRAY_TYPE"                , GL_COLOR_ARRAY_TYPE               },
  { "COLOR_ARRAY_STRIDE"              , GL_COLOR_ARRAY_STRIDE             },
  { "INDEX_ARRAY_TYPE"                , GL_INDEX_ARRAY_TYPE               },
  { "INDEX_ARRAY_STRIDE"              , GL_INDEX_ARRAY_STRIDE             },
  { "TEXTURE_COORD_ARRAY_SIZE"        , GL_TEXTURE_COORD_ARRAY_SIZE       },
  { "TEXTURE_COORD_ARRAY_TYPE"        , GL_TEXTURE_COORD_ARRAY_TYPE       },
  { "TEXTURE_COORD_ARRAY_STRIDE"      , GL_TEXTURE_COORD_ARRAY_STRIDE     },
  { "EDGE_FLAG_ARRAY_STRIDE"          , GL_EDGE_FLAG_ARRAY_STRIDE         },
  { "VERTEX_ARRAY_POINTER"            , GL_VERTEX_ARRAY_POINTER           },
  { "NORMAL_ARRAY_POINTER"            , GL_NORMAL_ARRAY_POINTER           },
  { "COLOR_ARRAY_POINTER"             , GL_COLOR_ARRAY_POINTER            },
  { "INDEX_ARRAY_POINTER"             , GL_INDEX_ARRAY_POINTER            },
  { "TEXTURE_COORD_ARRAY_POINTER"     , GL_TEXTURE_COORD_ARRAY_POINTER    },
  { "EDGE_FLAG_ARRAY_POINTER"         , GL_EDGE_FLAG_ARRAY_POINTER        },
  { "V2F"                             , GL_V2F                            },
  { "V3F"                             , GL_V3F                            },
  { "C4UB_V2F"                        , GL_C4UB_V2F                       },
  { "C4UB_V3F"                        , GL_C4UB_V3F                       },
  { "C3F_V3F"                         , GL_C3F_V3F                        },
  { "N3F_V3F"                         , GL_N3F_V3F                        },
  { "C4F_N3F_V3F"                     , GL_C4F_N3F_V3F                    },
  { "T2F_V3F"                         , GL_T2F_V3F                        },
  { "T4F_V4F"                         , GL_T4F_V4F                        },
  { "T2F_C4UB_V3F"                    , GL_T2F_C4UB_V3F                   },
  { "T2F_C3F_V3F"                     , GL_T2F_C3F_V3F                    },
  { "T2F_N3F_V3F"                     , GL_T2F_N3F_V3F                    },
  { "T2F_C4F_N3F_V3F"                 , GL_T2F_C4F_N3F_V3F                },
  { "T4F_C4F_N3F_V4F"                 , GL_T4F_C4F_N3F_V4F                },
#ifdef GL_EXT_vertex_array
  { "EXT_vertex_array"                , GL_EXT_vertex_array               },
#endif
#ifdef GL_EXT_bgra
  { "EXT_bgra"                        , GL_EXT_bgra                       },
#endif
#ifdef GL_EXT_paletted_texture
  { "EXT_paletted_texture"            , GL_EXT_paletted_texture           },
#endif
#ifdef GL_WIN_swap_hint
  { "WIN_swap_hint"                   , GL_WIN_swap_hint                  },
  { "WIN_draw_range_elements"         , GL_WIN_draw_range_elements        },
#endif
#ifdef GL_VERTEX_ARRAY_EXT
  { "VERTEX_ARRAY_EXT"                , GL_VERTEX_ARRAY_EXT               },
  { "NORMAL_ARRAY_EXT"                , GL_NORMAL_ARRAY_EXT               },
  { "COLOR_ARRAY_EXT"                 , GL_COLOR_ARRAY_EXT                },
  { "INDEX_ARRAY_EXT"                 , GL_INDEX_ARRAY_EXT                },
  { "TEXTURE_COORD_ARRAY_EXT"         , GL_TEXTURE_COORD_ARRAY_EXT        },
  { "EDGE_FLAG_ARRAY_EXT"             , GL_EDGE_FLAG_ARRAY_EXT            },
  { "VERTEX_ARRAY_SIZE_EXT"           , GL_VERTEX_ARRAY_SIZE_EXT          },
  { "VERTEX_ARRAY_TYPE_EXT"           , GL_VERTEX_ARRAY_TYPE_EXT          },
  { "VERTEX_ARRAY_STRIDE_EXT"         , GL_VERTEX_ARRAY_STRIDE_EXT        },
  { "VERTEX_ARRAY_COUNT_EXT"          , GL_VERTEX_ARRAY_COUNT_EXT         },
  { "NORMAL_ARRAY_TYPE_EXT"           , GL_NORMAL_ARRAY_TYPE_EXT          },
  { "NORMAL_ARRAY_STRIDE_EXT"         , GL_NORMAL_ARRAY_STRIDE_EXT        },
  { "NORMAL_ARRAY_COUNT_EXT"          , GL_NORMAL_ARRAY_COUNT_EXT         },
  { "COLOR_ARRAY_SIZE_EXT"            , GL_COLOR_ARRAY_SIZE_EXT           },
  { "COLOR_ARRAY_TYPE_EXT"            , GL_COLOR_ARRAY_TYPE_EXT           },
  { "COLOR_ARRAY_STRIDE_EXT"          , GL_COLOR_ARRAY_STRIDE_EXT         },
  { "COLOR_ARRAY_COUNT_EXT"           , GL_COLOR_ARRAY_COUNT_EXT          },
  { "INDEX_ARRAY_TYPE_EXT"            , GL_INDEX_ARRAY_TYPE_EXT           },
  { "INDEX_ARRAY_STRIDE_EXT"          , GL_INDEX_ARRAY_STRIDE_EXT         },
  { "INDEX_ARRAY_COUNT_EXT"           , GL_INDEX_ARRAY_COUNT_EXT          },
  { "TEXTURE_COORD_ARRAY_SIZE_EXT"    , GL_TEXTURE_COORD_ARRAY_SIZE_EXT   },
  { "TEXTURE_COORD_ARRAY_TYPE_EXT"    , GL_TEXTURE_COORD_ARRAY_TYPE_EXT   },
  { "TEXTURE_COORD_ARRAY_STRIDE_EXT"  , GL_TEXTURE_COORD_ARRAY_STRIDE_EXT },
  { "TEXTURE_COORD_ARRAY_COUNT_EXT"   , GL_TEXTURE_COORD_ARRAY_COUNT_EXT  },
  { "EDGE_FLAG_ARRAY_STRIDE_EXT"      , GL_EDGE_FLAG_ARRAY_STRIDE_EXT     },
  { "EDGE_FLAG_ARRAY_COUNT_EXT"       , GL_EDGE_FLAG_ARRAY_COUNT_EXT      },
  { "VERTEX_ARRAY_POINTER_EXT"        , GL_VERTEX_ARRAY_POINTER_EXT       },
  { "NORMAL_ARRAY_POINTER_EXT"        , GL_NORMAL_ARRAY_POINTER_EXT       },
  { "COLOR_ARRAY_POINTER_EXT"         , GL_COLOR_ARRAY_POINTER_EXT        },
  { "INDEX_ARRAY_POINTER_EXT"         , GL_INDEX_ARRAY_POINTER_EXT        },
  { "TEXTURE_COORD_ARRAY_POINTER_EXT" , GL_TEXTURE_COORD_ARRAY_POINTER_EXT},
  { "EDGE_FLAG_ARRAY_POINTER_EXT"     , GL_EDGE_FLAG_ARRAY_POINTER_EXT    },
#endif
#ifdef GL_BGR_EXT
  { "BGR_EXT"                         , GL_BGR_EXT                        },
  { "BGRA_EXT"                        , GL_BGRA_EXT                       },
#endif
#ifdef GL_COLOR_TABLE_FORMAT_EXT
  { "COLOR_TABLE_FORMAT_EXT"          , GL_COLOR_TABLE_FORMAT_EXT         },
  { "COLOR_TABLE_WIDTH_EXT"           , GL_COLOR_TABLE_WIDTH_EXT          },
  { "COLOR_TABLE_RED_SIZE_EXT"        , GL_COLOR_TABLE_RED_SIZE_EXT       },
  { "COLOR_TABLE_GREEN_SIZE_EXT"      , GL_COLOR_TABLE_GREEN_SIZE_EXT     },
  { "COLOR_TABLE_BLUE_SIZE_EXT"       , GL_COLOR_TABLE_BLUE_SIZE_EXT      },
  { "COLOR_TABLE_ALPHA_SIZE_EXT"      , GL_COLOR_TABLE_ALPHA_SIZE_EXT     },
  { "COLOR_TABLE_LUMINANCE_SIZE_EXT"  , GL_COLOR_TABLE_LUMINANCE_SIZE_EXT },
  { "COLOR_TABLE_INTENSITY_SIZE_EXT"  , GL_COLOR_TABLE_INTENSITY_SIZE_EXT },
#endif
#ifdef GL_COLOR_INDEX1_EXT
  { "COLOR_INDEX1_EXT"                , GL_COLOR_INDEX1_EXT               },
  { "COLOR_INDEX2_EXT"                , GL_COLOR_INDEX2_EXT               },
  { "COLOR_INDEX4_EXT"                , GL_COLOR_INDEX4_EXT               },
  { "COLOR_INDEX8_EXT"                , GL_COLOR_INDEX8_EXT               },
  { "COLOR_INDEX12_EXT"               , GL_COLOR_INDEX12_EXT              },
  { "COLOR_INDEX16_EXT"               , GL_COLOR_INDEX16_EXT              },
#endif
#ifdef GL_MAX_ELEMENTS_VERTICES_WIN
  { "MAX_ELEMENTS_VERTICES_WIN"       , GL_MAX_ELEMENTS_VERTICES_WIN      },
  { "MAX_ELEMENTS_INDICES_WIN"        , GL_MAX_ELEMENTS_INDICES_WIN       },
#endif
#ifdef GL_PHONG_WIN
  { "PHONG_WIN"                       , GL_PHONG_WIN                      },
  { "PHONG_HINT_WIN"                  , GL_PHONG_HINT_WIN                 },
#endif
#ifdef GL_FOG_SPECULAR_TEXTURE_WIN
  { "FOG_SPECULAR_TEXTURE_WIN"        , GL_FOG_SPECULAR_TEXTURE_WIN       },
#endif
  { "CURRENT_BIT"                     , GL_CURRENT_BIT                    },
  { "POINT_BIT"                       , GL_POINT_BIT                      },
  { "LINE_BIT"                        , GL_LINE_BIT                       },
  { "POLYGON_BIT"                     , GL_POLYGON_BIT                    },
  { "POLYGON_STIPPLE_BIT"             , GL_POLYGON_STIPPLE_BIT            },
  { "PIXEL_MODE_BIT"                  , GL_PIXEL_MODE_BIT                 },
  { "LIGHTING_BIT"                    , GL_LIGHTING_BIT                   },
  { "FOG_BIT"                         , GL_FOG_BIT                        },
  { "DEPTH_BUFFER_BIT"                , GL_DEPTH_BUFFER_BIT               },
  { "ACCUM_BUFFER_BIT"                , GL_ACCUM_BUFFER_BIT               },
  { "STENCIL_BUFFER_BIT"              , GL_STENCIL_BUFFER_BIT             },
  { "VIEWPORT_BIT"                    , GL_VIEWPORT_BIT                   },
  { "TRANSFORM_BIT"                   , GL_TRANSFORM_BIT                  },
  { "ENABLE_BIT"                      , GL_ENABLE_BIT                     },
  { "COLOR_BUFFER_BIT"                , GL_COLOR_BUFFER_BIT               },
  { "HINT_BIT"                        , GL_HINT_BIT                       },
  { "EVAL_BIT"                        , GL_EVAL_BIT                       },
  { "LIST_BIT"                        , GL_LIST_BIT                       },
  { "TEXTURE_BIT"                     , GL_TEXTURE_BIT                    },
  { "SCISSOR_BIT"                     , GL_SCISSOR_BIT                    },
  { "ALL_ATTRIB_BITS"                 , GL_ALL_ATTRIB_BITS                },
  { "CLIENT_PIXEL_STORE_BIT"          , GL_CLIENT_PIXEL_STORE_BIT         },
  { "CLIENT_VERTEX_ARRAY_BIT"         , GL_CLIENT_VERTEX_ARRAY_BIT        },
#ifdef GL_CLIENT_ALL_ATTRIB_BITS
  { "CLIENT_ALL_ATTRIB_BITS"          , GL_CLIENT_ALL_ATTRIB_BITS         },
#endif
  { 0, 0}
};

int luagl_checkboolean (lua_State *L, int narg) 
{
  int d = lua_toboolean(L, narg);
  if (d == 0 && !lua_isboolean(L, narg))  /* avoid extra test when d is not 0 */
    luaL_typerror(L, narg, lua_typename(L, LUA_TBOOLEAN));
  return d;
}

void* luagl_checkuserdata (lua_State *L, int narg) 
{
  if (!lua_islightuserdata(L, narg))
    luaL_typerror(L, narg, lua_typename(L, LUA_TLIGHTUSERDATA));
  return lua_touserdata(L, narg);
}

unsigned int luagl_get_gl_enum(lua_State *L, int index)
{
  return luagl_get_enum(L, index, luagl_const);
}

static const char *luagl_get_str_gl_enum(GLenum num)
{
  unsigned int i = 0;

  /* works only for simple enums */
  while(luagl_const[i].str != 0)
  {
    if(num == luagl_const[i].value)
      return luagl_const[i].str;

    i++;
  }
  return NULL;
}

static void luagl_pushenum(lua_State *L, GLenum num)
{
  const char* str = luagl_get_str_gl_enum(num);
  if (str)
    lua_pushstring(L, str);
  else
    lua_pushinteger(L, num);
}

/*Accum (op, value) -> none*/
static int luagl_accum(lua_State *L)
{
  glAccum(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*AlphaFunc (func, ref) -> none*/
static int luagl_alpha_func(lua_State *L)
{
  glAlphaFunc(luagl_get_gl_enum(L, 1), (GLclampf)luaL_checknumber(L, 2));
  return 0;
}

/*AreTexturesResident (texturesArray) -> residences*/
static int luagl_are_textures_resident(lua_State *L)
{
  GLboolean *residences;
  GLuint *textures;
  int n;

  n = luagl_get_arrayui(L, 1, &textures);

  residences = LUAGL_NEW_ARRAY(GLboolean, n);

  glAreTexturesResident(n, textures, residences);

  luagl_push_arrayb(L, residences, n);

  LUAGL_DELETE_ARRAY(textures);
  LUAGL_DELETE_ARRAY(residences);
  return 1;
}

/*ArrayElement (i) -> none*/
static int luagl_array_element(lua_State *L)
{
  glArrayElement(luaL_checkinteger(L, 1));
  return 0;
}

/*Begin (mode) -> none*/
static int luagl_begin(lua_State *L)
{
  glBegin(luagl_get_gl_enum(L, 1));
  return 0;
}

/*BindTexture (target, texture) -> none*/
static int luagl_bind_texture(lua_State *L)
{
  glBindTexture(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2));
  return 0;
}

/*Bitmap (xorig, yorig, xmove, ymove, bitmapArray) -> none*/
static int luagl_bitmap(lua_State *L)
{
  int width = 0, height = 0;
  GLubyte *bitmap = NULL;

  /* if no bitmap passed usefull to move raster pos */
  if (lua_istable(L,5))
  {
    height = luagl_get_array2uc(L, 5, &bitmap, &width);
    if (height==-1)
      luaL_argerror(L, 5, "must be a table of tables");
  }

  glBitmap(width, height, (GLfloat)luaL_checknumber(L, 1), (GLfloat)luaL_checknumber(L, 2),
           (GLfloat)luaL_checknumber(L, 3), (GLfloat)luaL_checknumber(L, 4), bitmap);

  LUAGL_DELETE_ARRAY(bitmap);
  return 0;
}

/*BitmapRaw (width, height, xorig, yorig, xmove, ymove, bitmap) -> none*/
static int luagl_bitmap_raw(lua_State *L)
{
  glBitmap(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), (GLfloat)luaL_checknumber(L, 3), 
           (GLfloat)luaL_checknumber(L, 4), (GLfloat)luaL_checknumber(L, 5), 
           (GLfloat)luaL_checknumber(L, 6), luagl_checkuserdata(L, 7));
  return 0;
}

/*BlendFunc (sfactor, dfactor) -> none*/
static int luagl_blend_func(lua_State *L)
{
  glBlendFunc(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*CallList (list) -> none*/
static int luagl_call_list(lua_State *L)
{
  glCallList(luaL_checkinteger(L, 1));
  return 0;
}

/*CallLists (listArray) -> none*/
static int luagl_call_lists(lua_State *L)
{
  GLsizei n;
  GLfloat *lists;

  n = luagl_get_arrayf(L, 1, &lists);

  glCallLists(n, GL_FLOAT, lists);

  LUAGL_DELETE_ARRAY(lists);
  return 0;
}

/*Clear (mask) -> none*/
static int luagl_clear(lua_State *L)
{
  glClear(luagl_get_gl_enum(L, 1));
  return 0;
}

/*ClearAccum (red, green, blue, alpha) -> none*/
static int luagl_clear_accum(lua_State *L)
{
  glClearAccum((GLfloat)luaL_checknumber(L, 1), (GLfloat)luaL_checknumber(L, 2),
               (GLfloat)luaL_checknumber(L, 3), (GLfloat)luaL_checknumber(L, 4));
  return 0;
}

/*ClearColor (red, green, blue, alpha) -> none*/
static int luagl_clear_color(lua_State *L)
{
  glClearColor((GLclampf)luaL_checknumber(L, 1), (GLclampf)luaL_checknumber(L, 2),
               (GLclampf)luaL_checknumber(L, 3), (GLclampf)luaL_checknumber(L, 4));
  return 0;
}

/*ClearDepth (depth) -> none*/
static int luagl_clear_depth(lua_State *L)
{
  glClearDepth((GLclampd)luaL_checknumber(L, 1));
  return 0;
}

/*ClearIndex (c) -> none*/
static int luagl_clear_index(lua_State *L)
{
  glClearIndex((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*ClearStencil (s) -> none*/
static int luagl_clear_stencil(lua_State *L)
{
  glClearStencil(luaL_checkinteger(L, 1));
  return 0;
}

/*ClipPlane (plane, equationArray) -> none*/
static int luagl_clip_plane(lua_State *L)
{
  GLdouble *equation;

  luagl_get_arrayd(L, 2, &equation);

  glClipPlane(luagl_get_gl_enum(L, 1), equation);

  LUAGL_DELETE_ARRAY(equation);
  return 0;
}

/*Color (red, green, blue [, alpha]) -> none
  Color (color) -> none*/
static int luagl_color(lua_State *L)
{
  if (lua_istable(L, 1))
  {
    GLdouble *parray = NULL;
    int n;

    n = luagl_get_arrayd(L, 1, &parray);
    if (n < 3)
    {
      LUAGL_DELETE_ARRAY(parray);
      luaL_argerror(L, 1, "invalid number of items in table (n<3).");
    }

    switch(n)
    {
    case 3:  glColor3dv(parray); break;
    case 4:  glColor4dv(parray); break;
    }

    LUAGL_DELETE_ARRAY(parray);
  }
  else
  {
    switch(lua_gettop(L))
    {
    case 3:  glColor3d(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
      break;
    case 4:  glColor4d(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
      break;
    }
  }

  return 0;
}

/*ColorMask (red, green, blue, alpha) -> none*/
static int luagl_color_mask(lua_State *L)
{
  glColorMask((GLboolean)luagl_checkboolean(L, 1), (GLboolean)luagl_checkboolean(L, 2),
              (GLboolean)luagl_checkboolean(L, 3), (GLboolean)luagl_checkboolean(L, 4));
  return 0;
}

/*ColorMaterial (face, mode) -> none*/
static int luagl_color_material(lua_State *L)
{
  glColorMaterial(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*ColorPointer (colorArray) -> none*/
static int luagl_color_pointer(lua_State *L)
{
  GLint size;
  static GLdouble *parray = NULL;

  LUAGL_DELETE_ARRAY(parray);

  if (lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayd(L, 1, &parray);
  }
  else 
  {
    int h = luagl_get_array2d(L, 1, &parray, &size);
    if (h==-1)
      luaL_argerror(L, 1, "must be a table of tables");
  }

  glColorPointer(size, GL_DOUBLE, 0, parray);
  return 0;
}

/*CopyPixels (x, y, width, height, type) -> none*/
static int luagl_copy_pixels(lua_State *L)
{
  glCopyPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
               luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), 
               luagl_get_gl_enum(L, 5));
  return 0;
}

/*CopyTexImage (level, internalFormat, border, x, y, width[, height]) -> none*/
static int luagl_copy_tex_image(lua_State *L)
{
  int num_args = lua_gettop(L);
  if (num_args > 6)
  {
    glCopyTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luagl_get_gl_enum(L, 2),
      luaL_checkinteger(L, 4), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 6), luaL_checkinteger(L, 7),
      luaL_checkinteger(L, 3));
  }
  else
  {
    glCopyTexImage1D(GL_TEXTURE_1D, luaL_checkinteger(L, 1), luagl_get_gl_enum(L, 2),
      luaL_checkinteger(L, 4), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 6), luaL_checkinteger(L, 3));
  }
  return 0;
}

/*CopyTexSubImage (level, x, y, xoffset, width[, yoffset, height]) -> none*/
static int luagl_copy_tex_sub_image(lua_State *L)
{
  int num_args = lua_gettop(L);
  if (num_args >= 7)
  {
    glCopyTexSubImage2D(GL_TEXTURE_2D,
      luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
      luaL_checkinteger(L, 6), luaL_checkinteger(L, 2),
      luaL_checkinteger(L, 3), luaL_checkinteger(L, 5),
      luaL_checkinteger(L, 7));
  }
  else
  {
    glCopyTexSubImage1D(GL_TEXTURE_1D,
      luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
      luaL_checkinteger(L, 2), luaL_checkinteger(L, 3),
      luaL_checkinteger(L, 5));
  }
  return 0;
}

/*CullFace (mode) -> none*/
static int luagl_cull_face(lua_State *L)
{
  glCullFace(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DeleteLists (list, range) -> none*/
static int luagl_delete_lists(lua_State *L)
{
  glDeleteLists(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
  return 0;
}

/*DeleteTextures (texturesArray) -> none*/
static int luagl_delete_textures(lua_State *L)
{
  int n;
  GLuint *textures;

  n = luagl_get_arrayui(L, 1, &textures);

  glDeleteTextures((GLsizei)n, (GLuint *)textures);

  LUAGL_DELETE_ARRAY(textures);

  return 0;
}

/*DepthFunc (func) -> none*/
static int luagl_depth_func(lua_State *L)
{
  glDepthFunc(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DepthMask (flag) -> none*/
static int luagl_depth_mask(lua_State *L)
{
  glDepthMask((GLboolean)luagl_checkboolean(L, 1));
  return 0;
}

/*DepthRange (zNear, zFar) -> none*/
static int luagl_depth_range(lua_State *L)
{
  glDepthRange((GLclampd)luaL_checknumber(L, 1), (GLclampd)luaL_checknumber(L, 2));
  return 0;
}

/*Disable (cap) -> none*/
static int luagl_disable(lua_State *L)
{
  glDisable(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DisableClientState (parray) -> none*/
static int luagl_disable_client_state(lua_State *L)
{
  glDisableClientState(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DrawArrays (mode, first, count) -> none*/
static int luagl_draw_arrays(lua_State *L)
{
  glDrawArrays(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

/*DrawBuffer (mode) -> none*/
static int luagl_draw_buffer(lua_State *L)
{
  glDrawBuffer(luagl_get_gl_enum(L, 1));
  return 0;
}

/*DrawElements (mode, indicesArray) -> none*/
static int luagl_draw_elements(lua_State *L)
{
  int n;
  GLuint *indices;

  n = luagl_get_arrayui(L, 2, &indices);

  glDrawElements(luagl_get_gl_enum(L, 1), n, GL_UNSIGNED_INT, indices);

  LUAGL_DELETE_ARRAY(indices);
  return 0;
}

/*DrawPixels (width, height, format, pixels) -> none*/
static int luagl_draw_pixels(lua_State *L)
{
  GLfloat *pixels;

  luagl_get_arrayf(L, 4, &pixels);

  glDrawPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
               luagl_get_gl_enum(L, 3), GL_FLOAT, pixels);

  LUAGL_DELETE_ARRAY(pixels);
  return 0;
}

/*DrawPixelsRaw (width, height, format, type, pixels) -> none*/
static int luagl_draw_pixels_raw(lua_State *L)
{
  glDrawPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
               luagl_get_gl_enum(L, 3), luagl_get_gl_enum(L, 4), luagl_checkuserdata(L, 5));
  return 0;
}

/*EdgeFlag (flag) -> none*/
static int luagl_edge_flag(lua_State *L)
{
  if (lua_istable(L, 1))
  {
    GLboolean *flag;
    luagl_get_arrayb(L, 1, &flag);

    glEdgeFlagv((GLboolean *)flag);

    LUAGL_DELETE_ARRAY(flag);
  }
  else 
    glEdgeFlag((GLboolean)luagl_checkboolean(L, 1));

  return 0;
}

/*EdgeFlagPointer (flagsArray) -> none*/
static int luagl_edge_flag_pointer(lua_State *L)
{
  static GLboolean *flags = 0;

  LUAGL_DELETE_ARRAY(flags);

  if(lua_isnil(L,1))
    return 0;

  luagl_get_arrayb(L, 1, &flags);

  glEdgeFlagPointer(0, flags);

  return 0;
}

/*Enable (cap) -> none*/
static int luagl_enable(lua_State *L)
{
  glEnable(luagl_get_gl_enum(L, 1));
  return 0;
}

/*EnableClientState (parray) -> none*/
static int luagl_enable_client_state(lua_State *L)
{
  glEnableClientState(luagl_get_gl_enum(L, 1));
  return 0;
}

/*End () -> none*/
static int luagl_end(lua_State *L)
{
  glEnd();
  return 0;
}

/*EndList () -> none*/
static int luagl_end_list(lua_State *L)
{
  glEndList();
  return 0;
}

/*EvalCoord (u[, v]) -> none
  EvalCoord (coordArray) -> none*/
static int luagl_eval_coord(lua_State *L)
{
  if (lua_istable(L, 1))
  {
    GLdouble *parray;
    if(luagl_get_arrayd(L, 1, &parray) == 1)
      glEvalCoord1dv(parray);
    else
      glEvalCoord2dv(parray);

    LUAGL_DELETE_ARRAY(parray);

    return 0;
  }
  else
  {
    int num_args = lua_gettop(L);
    switch(num_args)
    {
    case 1:  glEvalCoord1d(luaL_checknumber(L, 1));
      break;
    case 2:  glEvalCoord2d(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
      break;
    }
  }
  return 0;
}

/*EvalMesh (mode, i1, i2[,j1, j2]) -> none*/
static int luagl_eval_mesh(lua_State *L)
{
  int num_args = lua_gettop(L);
  if(num_args < 5)
    glEvalMesh1(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  else
    glEvalMesh2(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3),
                luaL_checkinteger(L, 4), luaL_checkinteger(L, 5));
  return 0;
}

/*EvalPoint (i[, j]) -> none*/
static int luagl_eval_point(lua_State *L)
{
  int num_args = lua_gettop(L);
  if(num_args == 1)
    glEvalPoint1(luaL_checkinteger(L, 1));
  else
    glEvalPoint2(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
  return 0;
}

/*FeedbackBuffer (size, type) -> dataArray*/
static int luagl_feedback_buffer(lua_State *L)
{
  GLfloat *buffer;
  GLsizei size;

  size = luaL_checkinteger(L, 1);

  buffer = LUAGL_NEW_ARRAY(GLfloat, size);

  glFeedbackBuffer (size, luagl_get_gl_enum(L, 2), buffer);

  luagl_push_arrayf(L, buffer, size);

  LUAGL_DELETE_ARRAY(buffer);

  return 1;
}

/*Finish () -> none*/
static int luagl_finish(lua_State *L)
{
  glFinish();
  return 0;
}

/*Flush () -> none*/
static int luagl_flush(lua_State *L)
{
  glFlush();
  return 0;
}

/*Fog (pname, param) -> none
  Fog (pname, paramsArray) -> none*/
static int luagl_fog(lua_State *L)
{
  GLfloat *param;

  if(lua_istable(L, 2))
  {
    luagl_get_arrayf(L, 2, &param);

    glFogfv(luagl_get_gl_enum(L, 1), (GLfloat*)param);

    LUAGL_DELETE_ARRAY(param);
    return 0;
  }
  else if(lua_isnumber(L, 2))
    glFogf(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  else 
    glFogi(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));

  return 0;
}

/*FrontFace (mode) -> none*/
static int luagl_front_face(lua_State *L)
{
  glFrontFace(luagl_get_gl_enum(L, 1));
  return 0;
}

/*Frustum (left, right, bottom, top, zNear, zFar) -> none*/
static int luagl_frustum(lua_State *L)
{
  glFrustum(luaL_checknumber(L, 1), luaL_checknumber(L, 2),
            luaL_checknumber(L, 3), luaL_checknumber(L, 4),
            luaL_checknumber(L, 5), luaL_checknumber(L, 6));
  return 0;
}

/*GenLists (range) -> num*/
static int luagl_gen_lists(lua_State *L)
{
  lua_pushnumber(L, glGenLists(luaL_checkinteger(L, 1)) );
  return 1;
}

/*GenTextures (n) -> texturesArray*/
static int luagl_gen_textures(lua_State *L)
{
  GLsizei size;
  GLuint *textures;

  size = luaL_checkinteger(L, 1);
  textures = LUAGL_NEW_ARRAY(GLuint, size);

  glGenTextures(size, textures);

  luagl_push_arrayui(L, textures, size);

  LUAGL_DELETE_ARRAY(textures);

  return 1;
}

/*Get (pname) -> params*/
static int luagl_get(lua_State *L)
{
  int i, size=1;
  GLenum e;
  GLdouble *params;
  int mask;

  e = luagl_get_gl_enum(L, 1);

  switch(e)
  {
  case GL_STENCIL_VALUE_MASK:
  case GL_LINE_STIPPLE_PATTERN:
  case GL_STENCIL_WRITEMASK:
  case GL_INDEX_WRITEMASK:
    mask = 0;
    glGetIntegerv(e, &mask);
    lua_pushstring(L, luagl_mask2str(mask));
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

  default:
    luaL_argerror(L, 1, "unknown enumeration.");
    break;
  }

  params = LUAGL_NEW_ARRAY(GLdouble, size);

  glGetDoublev(e, params);

  for(i = 0; i < size; i++)
    lua_pushnumber(L, params[i]);

  LUAGL_DELETE_ARRAY(params);

  return size;
}

/*GetConst (pname) -> constant string*/
static int luagl_get_const(lua_State *L)
{
  int i, size=1;
  GLenum e;
  GLenum *params;

  e = luagl_get_gl_enum(L, 1);

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

  params = LUAGL_NEW_ARRAY(GLenum, size);

  glGetIntegerv(e, (GLint*)params);

  for(i = 0; i < size; i++)
    luagl_pushenum(L, params[i]);

  LUAGL_DELETE_ARRAY(params);

  return size;
}

/*GetArray (pname) -> paramsArray*/
static int luagl_get_array(lua_State *L)
{
  int size = 1;
  GLenum e;
  GLdouble *params;

  e = luagl_get_gl_enum(L, 1);

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

  params = LUAGL_NEW_ARRAY(GLdouble, size);

  glGetDoublev(e, params);

  luagl_push_arrayd(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetClipPlane (plane) -> equationArray*/
static int luagl_get_clip_plane(lua_State *L)
{
  GLdouble *equation;

  equation = LUAGL_NEW_ARRAY(GLdouble, 4);

  glGetClipPlane(luagl_get_gl_enum(L, 1), equation);

  luagl_push_arrayd(L, equation, 4);

  LUAGL_DELETE_ARRAY(equation);

  return 1;
}

/*GetError () -> error flag*/
static int luagl_get_error(lua_State *L)
{
  GLenum error = glGetError();
  if(error == GL_NO_ERROR)
    lua_pushnil(L);
  else
    luagl_pushenum(L, error);
  return 1;
}

/*GetLight (light, pname) -> paramsArray*/
static int luagl_get_light(lua_State *L)
{
  int size = 1;
  GLenum e1, e2;
  GLfloat *params;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

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

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetLightfv(e1, e2, params);

  luagl_push_arrayf(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetMap (target, query) -> vArray*/
static int luagl_get_map(lua_State *L)
{
  int size = 1;
  GLenum e1, e2;
  GLdouble *params;
  GLint *order;

  order = LUAGL_NEW_ARRAY(GLint, 2);
  order[0] = order[1] = 1;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

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

  params = LUAGL_NEW_ARRAY(GLdouble, size);

  glGetMapdv(e1, e2, params);

  luagl_push_arrayd(L, params, size);

  LUAGL_DELETE_ARRAY(params);
  LUAGL_DELETE_ARRAY(order);

  return 1;
}

/*GetMaterial (face, pname) -> paramsArray*/
static int luagl_get_material(lua_State *L)
{
  int size = 1;
  GLenum e1, e2;
  GLfloat *params;

  /* get string parameters */
  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

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

  params = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetMaterialfv(e1, e2, params);

  luagl_push_arrayf(L, params, size);

  LUAGL_DELETE_ARRAY(params);

  return 1;
}

/*GetPixelMap (map) -> valuesArray*/
static int luagl_get_pixel_map(lua_State *L)
{
  int size;
  int s = GL_PIXEL_MAP_R_TO_R_SIZE;
  GLenum e;
  GLfloat *values;

  e = luagl_get_gl_enum(L, 1);

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

  values = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetPixelMapfv(e, values);

  luagl_push_arrayf(L, values, size);

  LUAGL_DELETE_ARRAY(values);

  return 1;
}

/*GetPointer (pname, n) -> valuesArray*/
static int luagl_get_pointer(lua_State *L)
{
  int n;
  GLenum e;

  e = luagl_get_gl_enum(L, 1);
  n = luaL_checkinteger(L, 2);

  if(e == GL_EDGE_FLAG_ARRAY_POINTER)
  {
    GLboolean *flags;
    glGetPointerv(e, (void *)&flags);
    if(flags == 0)
      return 0;

    luagl_push_arrayb(L, flags, n);
  }
  else
  {
    GLdouble *params;
    glGetPointerv(e, (void *)&params);
    if(params == 0)
      return 0;

    luagl_push_arrayd(L, params, n);
  }

  return 1;
}

/*GetPolygonStipple () -> maskArray*/
static int luagl_get_polygon_stipple(lua_State *L)
{
  GLubyte *mask = LUAGL_NEW_ARRAY(GLubyte, 1024);

  glGetPolygonStipple(mask);

  luagl_push_arrayuc(L, mask, 1024);

  LUAGL_DELETE_ARRAY(mask);

  return 1;
}

/*GetString (name) -> string*/
static int luagl_get_string(lua_State *L)
{
  lua_pushstring(L, (const char*)glGetString(luagl_get_gl_enum(L, 1)));
  return 1;
}

/*GetTexEnv (pname) -> paramsArray*/
static int luagl_get_tex_env(lua_State *L)
{
  GLenum e1;

  e1 = luagl_get_gl_enum(L, 1);

  if (e1 != GL_TEXTURE_ENV_MODE)
  {
    GLfloat *params;

    params = LUAGL_NEW_ARRAY(GLfloat, 4);

    glGetTexEnvfv(GL_TEXTURE_ENV, e1, params);

    luagl_push_arrayf(L, params, 4);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
  {
    GLint e2;
    glGetTexEnviv(GL_TEXTURE_ENV, e1, &e2);
    luagl_pushenum(L, e2);
  }

  return 1;
}

/*GetTexGen (coord, pname) -> paramsArray*/
static int luagl_get_tex_gen(lua_State *L)
{
  GLenum e2;

  e2 = luagl_get_gl_enum(L, 2);

  if (e2 != GL_TEXTURE_GEN_MODE)
  {
    GLdouble *params;

    params = LUAGL_NEW_ARRAY(GLdouble, 4);

    glGetTexGendv(luagl_get_gl_enum(L, 1), e2, params);

    luagl_push_arrayd(L, params, 4);

    LUAGL_DELETE_ARRAY(params);
  }
  else
  {
    GLint e3;
    glGetTexGeniv(luagl_get_gl_enum(L, 1), e2, &e3);
    luagl_pushenum(L, e3);
  }
  return 1;
}

static int luagl_get_depth(GLenum format)
{
  int depth = 0;
  switch(format)
  {
  case GL_DEPTH_COMPONENT:
  case GL_STENCIL_INDEX:
  case GL_COLOR_INDEX:
  case GL_RED:
  case GL_GREEN:
  case GL_BLUE:
  case GL_ALPHA:
  case GL_LUMINANCE:
    depth = 1;
    break;

  case GL_LUMINANCE_ALPHA:
    depth = 2;
    break;

  case GL_RGB:
#ifdef GL_BGR_EXT
  case GL_BGR_EXT:
#endif
    depth = 3;
    break;

  case GL_RGBA:
#ifdef GL_BGRA_EXT
  case GL_BGRA_EXT:
#endif
    depth = 4;
    break;
  }

  return depth;
}

/*GetTexImage (target, level, format) -> pixelsArray*/
static int luagl_get_tex_image(lua_State *L)
{
  int depth, size;
  int width, height, level;
  GLenum target, format;
  GLfloat *pixels;

  target = luagl_get_gl_enum(L, 1);
  level = luaL_checkinteger(L, 2);
  format = luagl_get_gl_enum(L, 3);

  /* get width and height of image */
  glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &height);

  depth = luagl_get_depth(format);
  if (depth == 0)
    luaL_argerror(L, 3, "unknown format");

  size = depth * width * height;
  pixels = LUAGL_NEW_ARRAY(GLfloat, size);

  glGetTexImage(target, level, format, GL_FLOAT, pixels);

  luagl_push_arrayf(L, pixels, size);

  LUAGL_DELETE_ARRAY(pixels);

  return 1;
}

/*GetTexImageRaw (target, level, format, type, pixels) -> none*/
static int luagl_get_tex_image_raw(lua_State *L)
{
  glGetTexImage(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luagl_get_gl_enum(L, 3), 
                luagl_get_gl_enum(L, 4), luagl_checkuserdata(L, 5));
  return 0;
}

/*GetTexLevelParameter (target, level, pname) -> param*/
static int luagl_get_tex_level_parameter(lua_State *L)
{
  GLfloat param;
  glGetTexLevelParameterfv(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luagl_get_gl_enum(L, 3), &param);
  lua_pushnumber(L, param);
  return 1;
}

/*GetTexParameter (target, pname) -> paramsArray*/
static int luagl_get_tex_parameter(lua_State *L)
{
  GLenum target, pname;

  target = luagl_get_gl_enum(L, 1);
  pname = luagl_get_gl_enum(L, 2);

  if(pname == GL_TEXTURE_BORDER_COLOR)
  {
    GLfloat *params;

    params = LUAGL_NEW_ARRAY(GLfloat, 4);

    glGetTexParameterfv(target, pname, params);

    luagl_push_arrayf(L, params, 4);

    LUAGL_DELETE_ARRAY(params);
  }
  else if(pname == GL_TEXTURE_PRIORITY)
  {
    GLfloat param;
    glGetTexParameterfv(target, pname, &param);
    lua_pushnumber(L, param);
  }
  else
  {
    GLint e;
    glGetTexParameteriv(target, pname, &e);
    luagl_pushenum(L, e);
  }
  return 1;
}

/*Hint (target, mode) -> none*/
static int luagl_hint(lua_State *L)
{
  glHint(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*Index (c) -> none*/
static int luagl_index(lua_State *L)
{
  if (lua_istable(L, 1))
  {
    GLdouble *c;

    luagl_get_arrayd(L, 1, &c);

    glIndexdv((GLdouble *)c);

    LUAGL_DELETE_ARRAY(c);
  }
  else 
    glIndexd(luaL_checknumber(L, 1));

  return 0;
}

/*IndexMask (mask) -> none*/
static int luagl_index_mask(lua_State *L)
{
  if(lua_type(L,1) == LUA_TSTRING)
    glIndexMask(luagl_str2mask(luaL_checkstring(L, 1)));
  else 
    glIndexMask(luaL_checkinteger(L, 1));
  return 0;
}

/*IndexPointer (indexArray) -> none*/
static int luagl_index_pointer(lua_State *L)
{
  static GLdouble *parray = NULL;

  LUAGL_DELETE_ARRAY(parray);

  /* Used to release the static memory */
  if(lua_isnil(L,1))
    return 0;

  luagl_get_arrayd(L, 1, &parray);

  glIndexPointer(GL_DOUBLE, 0, parray);

  return 0;
}

/*InitNames () -> none*/
static int luagl_init_names(lua_State *L)
{
  glInitNames();
  return 0;
}

/*IsEnabled (cap) -> true/false*/
static int luagl_is_enabled(lua_State *L)
{
  lua_pushboolean(L, glIsEnabled(luagl_get_gl_enum(L, 1)));
  return 1;
}

/*IsList (list) -> true/false*/
static int luagl_is_list(lua_State *L)
{
  lua_pushboolean(L, glIsList(luaL_checkinteger(L, 1)));
  return 1;
}

/*IsTexture (texture) -> true/false*/
static int luagl_is_texture(lua_State *L)
{
  lua_pushboolean(L, glIsTexture(luaL_checkinteger(L, 1)));
  return 1;
}

/*Light (light, pname, param) -> none
  Light (light, pname, paramsArray) -> none*/
static int luagl_light(lua_State *L)
{
  if(lua_istable(L, 3))
  {
    GLfloat *params;
    luagl_get_arrayf(L, 3, &params);

    glLightfv(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glLightf(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), (GLfloat)luaL_checknumber(L, 3));
  return 0;
}

/*LightModel (pname, param) -> none
  LightModel (pname, paramsArray) -> none*/
static int luagl_light_model(lua_State *L)
{
  if (lua_istable(L, 2))
  {
    GLfloat *params;
    luagl_get_arrayf(L, 2, &params);

    glLightModelfv(luagl_get_gl_enum(L, 1), (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glLightModelf(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*LineStipple (factor, pattern) -> none*/
static int luagl_line_stipple(lua_State *L)
{
  if (lua_type(L,2) == LUA_TSTRING)
    glLineStipple(luaL_checkinteger(L, 1), (GLushort)luagl_str2mask(luaL_checkstring(L, 2)));
  else 
    glLineStipple(luaL_checkinteger(L, 1), (GLushort)luaL_checkinteger(L, 2));
  return 0;
}

/*LineWidth (width) -> none*/
static int luagl_line_width(lua_State *L)
{
  glLineWidth((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*ListBase (base) -> none*/
static int luagl_list_base(lua_State *L)
{
  glListBase(luaL_checkinteger(L, 1));
  return 0;
}

/*LoadIdentity () -> none*/
static int luagl_load_identity(lua_State *L)
{
  glLoadIdentity();
  return 0;
}

/*LoadMatrix (mArray) -> none*/
static int luagl_load_matrix(lua_State *L)
{
  GLdouble *m;
  int n;

  n = luagl_get_arrayd(L, 1, &m);
  if (n < 16)
  {
    LUAGL_DELETE_ARRAY(m);
    luaL_argerror(L, 1, "invalid number of elements in the matrix table (n<16).");
  }

  glLoadMatrixd(m);

  LUAGL_DELETE_ARRAY(m);

  return 0;
}

/*LoadName (name) -> none*/
static int luagl_load_name(lua_State *L)
{
  glLoadName(luaL_checkinteger(L, 1));
  return 0;
}

/*LogicOp (opcode) -> none*/
static int luagl_logic_op(lua_State *L)
{
  glLogicOp(luagl_get_gl_enum(L, 1));
  return 0;
}

/*Map (target, u1, u2, ustride, pointsArray) -> none
  Map (target, u1, u2, ustride, v1, v2, vstride, pointsArray) -> none*/
static int luagl_map(lua_State *L)
{
  int size=1;
  GLenum target;
  GLdouble *points;
  GLint uorder, vorder;

  target = luagl_get_gl_enum(L, 1);

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

  if (lua_gettop(L) < 6)
  {
    uorder = luagl_get_arrayd(L, 4, &points) / size;

    glMap1d(target, luaL_checknumber(L, 2),
      luaL_checknumber(L, 3),
      size, uorder, points);

    LUAGL_DELETE_ARRAY(points);
  }
  else
  {
    vorder = luagl_get_array2d(L, 6, &points, &uorder);
    uorder /= size;

    if (vorder==-1)
      luaL_argerror(L, 6, "must be a table of tables");

    glMap2d(target, luaL_checknumber(L, 2), luaL_checknumber(L, 3),
            size, uorder, luaL_checknumber(L, 4), luaL_checknumber(L, 5),
            size * uorder, vorder, points);

    LUAGL_DELETE_ARRAY(points);
  }
  return 0;
}

/*MapGrid (un, u1, u2[, vn, v1, v2]) -> none*/
static int luagl_map_grid(lua_State *L)
{
  if (lua_gettop(L) < 6)
    glMapGrid1d(luaL_checkinteger(L, 1),
                luaL_checknumber(L, 2),
                luaL_checknumber(L, 3));
  else
    glMapGrid2d(luaL_checkinteger(L, 1),
                luaL_checknumber(L, 2),
                luaL_checknumber(L, 3),
                luaL_checkinteger(L, 4),
                luaL_checknumber(L, 5),
                luaL_checknumber(L, 6));
  return 0;
}

/*Material (face, pname, param) -> none*/
static int luagl_material(lua_State *L)
{
  GLenum e1, e2;
  GLfloat *params;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  if (lua_istable(L, 3))
  {
    luagl_get_arrayf(L, 3, &params);

    glMaterialfv(e1, e2, (GLfloat *)params);

    LUAGL_DELETE_ARRAY(params);
  }
  else 
    glMaterialf(e1, e2, (GLfloat)luaL_checknumber(L, 3));
  return 0;
}

/*MatrixMode (mode) -> none*/
static int luagl_matrix_mode(lua_State *L)
{
  glMatrixMode(luagl_get_gl_enum(L, 1));
  return 0;
}

/*MultMatrix (mArray) -> none*/
static int luagl_mult_matrix(lua_State *L)
{
  GLdouble *m;
  int n;

  n = luagl_get_arrayd(L, 1, &m);
  if (n < 16)
  {
    LUAGL_DELETE_ARRAY(m);
    luaL_argerror(L, 1, "invalid number of elements in the matrix table (n<16).");
  }

  glMultMatrixd(m);

  LUAGL_DELETE_ARRAY(m);

  return 0;
}

/*NewList (list, mode) -> none*/
static int luagl_new_list(lua_State *L)
{
  glNewList(luaL_checkinteger(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*Normal (nx, ny, nz) -> none
  Normal (nArray) -> none*/
static int luagl_normal(lua_State *L)
{
  if(lua_istable(L, 1))
  {
    GLdouble *parray;
    int n;

    n = luagl_get_arrayd(L, 1, &parray);
    if (n < 3)
      luaL_argerror(L, 1, "invalid number of elements in the table (n<3)");

    glNormal3dv(parray);

    LUAGL_DELETE_ARRAY(parray);
  }
  else
    glNormal3d(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));

  return 0;
}

/*NormalPointer (normalArray) -> none*/
static int luagl_normal_pointer(lua_State *L)
{
  GLint size;
  static GLdouble *parray = NULL;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayd(L, 1, &parray);
  }
  else 
  {
    int h = luagl_get_array2d(L, 1, &parray, &size);
    if (h==-1)
      luaL_argerror(L, 1, "must be a table of tables");
  }

  glNormalPointer(GL_DOUBLE, 0, parray);

  return 0;
}

/*Ortho (left, right, bottom, top, zNear, zFar) -> none*/
static int luagl_ortho(lua_State *L)
{
  glOrtho(luaL_checknumber(L, 1), luaL_checknumber(L, 2),
          luaL_checknumber(L, 3), luaL_checknumber(L, 4),
          luaL_checknumber(L, 5), luaL_checknumber(L, 6));
  return 0;
}

/*PassThrough (token) -> none*/
static int luagl_pass_through(lua_State *L)
{
  glPassThrough((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*PixelMap (map, valuesArray) -> none*/
static int luagl_pixel_map(lua_State *L)
{
  GLfloat *values;
  int mapsize;

  mapsize = luagl_get_arrayf(L, 2, &values);

  glPixelMapfv(luagl_get_gl_enum(L, 1), mapsize, values);

  LUAGL_DELETE_ARRAY(values);

  return 0;
}

/*PixelStore (pname, param) -> none*/
static int luagl_pixel_store(lua_State *L)
{
  if (lua_isboolean(L,2))
    glPixelStoref(luagl_get_gl_enum(L, 1), (GLfloat)luagl_checkboolean(L, 2));
  else 
    glPixelStoref(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PixelTransfer (pname, param) -> none*/
static int luagl_pixel_transfer(lua_State *L)
{
  if(lua_isboolean(L,2))
    glPixelTransferf(luagl_get_gl_enum(L, 1), (GLfloat)luagl_checkboolean(L, 2));
  else 
    glPixelTransferf(luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PixelZoom (xfactor, yfactor) -> none*/
static int luagl_pixel_zoom(lua_State *L)
{
  glPixelZoom((GLfloat)luaL_checknumber(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PointSize (size) -> none*/
static int luagl_point_size(lua_State *L)
{
  glPointSize((GLfloat)luaL_checknumber(L, 1));
  return 0;
}

/*PolygonMode (face, mode) -> none*/
static int luagl_polygon_mode(lua_State *L)
{
  glPolygonMode(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));
  return 0;
}

/*PolygonOffset (factor, units) -> none*/
static int luagl_polygon_offset(lua_State *L)
{
  glPolygonOffset((GLfloat)luaL_checknumber(L, 1), (GLfloat)luaL_checknumber(L, 2));
  return 0;
}

/*PolygonStipple (maskArray) -> none*/
static int luagl_polygon_stipple(lua_State *L)
{
  GLubyte *parray;
  int width, height = 32;

  height = luagl_get_array2uc(L, 1, &parray, &width);
  if (height == -1)  /* if not 2D, get 1D */
    width = luagl_get_arrayuc(L, 1, &parray);

  if (width != 32 && height != 32)
  {
    LUAGL_DELETE_ARRAY(parray);
    luaL_argerror(L, 1, "invalid stipple size (width != 32 && height != 32)");
  }

  glPolygonStipple(parray);
  LUAGL_DELETE_ARRAY(parray);

  return 0;
}

/*PopAttrib () -> none*/
static int luagl_pop_attrib(lua_State *L)
{
  glPopAttrib();
  return 0;
}

/*PopClientAttrib () -> none*/
static int luagl_pop_client_attrib(lua_State *L)
{
  glPopClientAttrib();
  return 0;
}

/*PopMatrix () -> none*/
static int luagl_pop_matrix(lua_State *L)
{
  glPopMatrix();
  return 0;
}

/*PopName () -> none*/
static int luagl_pop_name(lua_State *L)
{
  glPopName();
  return 0;
}

/*PrioritizeTextures (texturesArray, prioritiesArray) -> none*/
static int luagl_prioritize_textures(lua_State *L)
{
  GLsizei n1, n2;
  GLuint *array1;
  GLclampf *array2;

  n1 = luagl_get_arrayui(L, 1, &array1);
  n2 = luagl_get_arrayf(L, 2, &array2);

  if (n1 > n2) n1 =  n2;

  glPrioritizeTextures(n1, array1, array2);

  LUAGL_DELETE_ARRAY(array1);
  LUAGL_DELETE_ARRAY(array2);

  return 0;
}

/*PushAttrib (mask) -> none*/
static int luagl_push_attrib(lua_State *L)
{
  glPushAttrib(luagl_get_gl_enum(L, 1));
  return 0;
}

/*PushClientAttrib (mask) -> none*/
static int luagl_push_client_attrib(lua_State *L)
{
  glPushClientAttrib(luagl_get_gl_enum(L, 1));
  return 0;
}

/*PushMatrix () -> none*/
static int luagl_push_matrix(lua_State *L)
{
  glPushMatrix();
  return 0;
}

/*PushName (GLuint name) -> none*/
static int luagl_push_name(lua_State *L)
{
  glPushName(luaL_checkinteger(L, 1));
  return 0;
}

/*RasterPos (x, y[, z, w]) -> none
  RasterPos (vArray) -> none*/
static int luagl_raster_pos(lua_State *L)
{
  if(lua_istable(L, 1))
  {
    GLdouble *parray;
    int n = luagl_get_arrayd(L, 1, &parray);
    if (n < 2)
    {
      LUAGL_DELETE_ARRAY(parray);
      luaL_argerror(L, 1, "invalid number of items in table (n<2).");
    }

    switch(n)
    {
    case 2:  glRasterPos2dv(parray); break;
    case 3:  glRasterPos3dv(parray); break;
    case 4:  glRasterPos4dv(parray); break;
    }

    LUAGL_DELETE_ARRAY(parray);
  }
  else
  {
    int num_args = lua_gettop(L);
    switch(num_args)
    {
    case 2:  
      glRasterPos2d(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
      break;
    case 3:  
      glRasterPos3d(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
      break;
    case 4:  
      glRasterPos4d(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
      break;
    }
  }
  return 0;
}

/*ReadBuffer (mode) -> none*/
static int luagl_read_buffer(lua_State *L)
{
  glReadBuffer(luagl_get_gl_enum(L, 1));
  return 0;
}

/*ReadPixels (x, y, width, height, format) -> pixelsArray */
static int luagl_read_pixels(lua_State *L)
{
  GLenum format;
  GLfloat *pixels;
  int width, height, size, depth=1;

  format = luagl_get_gl_enum(L, 5);
  depth = luagl_get_depth(format);
  if (depth == 0)
    luaL_argerror(L, 5, "unknown format");

  width = luaL_checkinteger(L, 3);
  height = luaL_checkinteger(L, 4);
  size = width*height*depth;

  pixels = LUAGL_NEW_ARRAY(GLfloat, size);

  glReadPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
               width, height, format, GL_FLOAT, pixels);

  luagl_push_arrayf(L, pixels, size);

  LUAGL_DELETE_ARRAY(pixels);

  return 1;
}

/*ReadPixelsRaw (x, y, width, height, format, type, pixels) -> none*/
static int luagl_read_pixels_raw(lua_State *L)
{
  glReadPixels(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
               luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), 
               luagl_get_gl_enum(L, 5), luagl_get_gl_enum(L, 6), luagl_checkuserdata(L, 7));
  return 0;
}

/*Rect (x1, y1, x2, y2) -> none
  Rect (v1, v2) -> none*/
static int luagl_rect(lua_State *L)
{
  GLdouble *v1, *v2;

  if (lua_istable(L, 1) && lua_istable(L, 2))
  {
    luagl_get_arrayd(L, 1, &v1);
    luagl_get_arrayd(L, 2, &v2);

    glRectdv(v1, v2);

    LUAGL_DELETE_ARRAY(v1);
    LUAGL_DELETE_ARRAY(v2);
  }
  else 
    glRectd(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/*RenderMode (mode) -> none*/
static int luagl_render_mode(lua_State *L)
{
  lua_pushnumber(L, glRenderMode(luagl_get_gl_enum(L, 1)));
  return 1;
}

/*Rotate (angle, x, y, z) -> none*/
static int luagl_rotate(lua_State *L)
{
  glRotated(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  return 0;
}

/*Scale (x, y, z) -> none*/
static int luagl_scale(lua_State *L)
{
  glScaled(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  return 0;
}

/*Scissor (x, y, width, height) -> none*/
static int luagl_scissor(lua_State *L)
{
  glScissor(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  return 0;
}

/*SelectBuffer (size) -> SelectArray*/
static int luagl_select_buffer(lua_State *L)
{
  int size;
  GLuint *buffer;

  size = luaL_checkinteger(L, 1);

  buffer = LUAGL_NEW_ARRAY(GLuint, size+1); /*first index is size*/
  buffer[0]=(GLuint)size;

  glSelectBuffer(size, buffer+1);

  lua_pushlightuserdata(L,buffer);
  return 1;
}

static int luagl_get_select_buffer(lua_State *L)
{
  GLuint* buffer = (GLuint*)luagl_checkuserdata(L, 1);
  int i = luaL_checkinteger(L,2);
  if (buffer) 
  { 
    int size = (int)buffer[0];
    if ((i<=size) && (i>0)) 
    {
      lua_pushnumber(L,buffer[i]); /*select buffer data begin at index i */
      return 1;
     }
  }
  return 0;
}

static int luagl_free_select_buffer(lua_State *L)
{
  GLuint* buffer = (GLuint*)luagl_checkuserdata(L, 1);
  LUAGL_DELETE_ARRAY(buffer);
  return 0;
}

static int luagl_new_data(lua_State *L)
{
  int size = luaL_checkinteger(L, 1);
  void* buffer = LUAGL_NEW_ARRAY(unsigned char, size);
  lua_pushlightuserdata(L, buffer);
  return 1;
}

static int luagl_free_data(lua_State *L)
{
  void* buffer = luagl_checkuserdata(L, 1);
  LUAGL_DELETE_ARRAY(buffer);
  return 0;
}

/*ShadeModel (mode) -> none*/
static int luagl_shade_model(lua_State *L)
{
  glShadeModel(luagl_get_gl_enum(L, 1));
  return 0;
}

/*StencilFunc (func, ref, mask) -> none*/
static int luagl_stencil_func(lua_State *L)
{
  if (lua_type(L,3) == LUA_TSTRING)
    glStencilFunc(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luagl_str2mask(luaL_checkstring(L, 3)));
  else 
    glStencilFunc(luagl_get_gl_enum(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  return 0;
}

/*StencilMask (mask) -> none*/
static int luagl_stencil_mask(lua_State *L)
{
  if(lua_type(L,1) == LUA_TSTRING)
    glStencilMask(luagl_str2mask(luaL_checkstring(L, 1)));
  else 
    glStencilMask(luaL_checkinteger(L, 1));
  return 0;
}

/*StencilOp (fail, zfail, zpass) -> none*/
static int luagl_stencil_op(lua_State *L)
{
  glStencilOp(luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2), luagl_get_gl_enum(L, 3));
  return 0;
}

/*TexCoord (s[, t, r, q]) -> none
  TexCoord (vArray) -> none*/
static int luagl_tex_coord(lua_State *L)
{
  int index;
  int num_args = lua_gettop(L);

  GLdouble *v = 0;

  if (lua_istable(L, 1))
    num_args = luagl_get_arrayd(L, 1, &v);
  else
  {
    v = LUAGL_NEW_ARRAY(GLdouble, num_args);

    for(index = 0; index < num_args; index++)
    {
      v[index] = luaL_checknumber(L, index+1);
    }
  }

  switch(num_args)
  {
  case 1:  glTexCoord1dv((GLdouble *)v);  break;
  case 2:  glTexCoord2dv((GLdouble *)v);  break;
  case 3:  glTexCoord3dv((GLdouble *)v);  break;
  case 4:  glTexCoord4dv((GLdouble *)v);  break;
  }

  LUAGL_DELETE_ARRAY(v);

  return 0;
}

/*TexCoordPointer(vArray) -> none*/
static int luagl_tex_coord_pointer(lua_State *L)
{
  GLint size;
  static GLdouble *parray = NULL;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayd(L, 1, &parray);
  }
  else
  {
    int h = luagl_get_array2d(L, 1, &parray, &size);
    if (h==-1)
      luaL_argerror(L, 1, "must be a table of tables");
  }

  glTexCoordPointer(size, GL_DOUBLE, 0, parray);

  return 0;
}

/*TexEnv (pname, param) -> none
  TexEnv (pname, paramsArray) -> none*/
static int luagl_tex_env(lua_State *L)
{
  if(lua_istable(L, 2))
  {
    GLfloat *param;
    luagl_get_arrayf(L, 2, &param);

    glTexEnvfv(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), (GLfloat *)param);

    LUAGL_DELETE_ARRAY(param);
  }
  else if(lua_isnumber(L, 2))
    glTexEnvf(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), (GLfloat)luaL_checknumber(L, 2));
  else 
    glTexEnvi(GL_TEXTURE_ENV, luagl_get_gl_enum(L, 1), luagl_get_gl_enum(L, 2));

  return 0;
}

/*TexGen (coord, pname, param) -> none
  TexGen (coord, pname, paramsArray) -> none*/
static int luagl_tex_gen(lua_State *L)
{
  GLenum e1, e2;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  if (lua_istable(L, 3))
  {
    GLdouble *param;
    luagl_get_arrayd(L, 3, &param);

    glTexGendv(e1, e2, (GLdouble *)param);

    LUAGL_DELETE_ARRAY(param);
  }
  else 
    glTexGeni(e1, e2, luagl_get_gl_enum(L, 3));

  return 0;
}

/*TexImage(level, internalformat, format, pixels) -> none*/
static int luagl_tex_image(lua_State *L)
{
  GLenum e;
  GLubyte *pixels;
  GLsizei width, height;
  int iformat;
  int index;

  if(lua_isnumber(L, 1) && lua_istable(L, 2))
  {
    /* undocumented parameter passing, 
       so it can be compatible with a texture created for glu.Build2DMipmaps */
    lua_getfield(L, 2, "components");
    iformat = luaL_checkinteger(L, -1);
    lua_remove(L, -1);

    lua_getfield(L, 2, "format");
    e = luagl_get_gl_enum(L, -1);
    lua_remove(L, -1);

    index = 2;
  }
  else
  {
    e = luagl_get_gl_enum(L, 3);
    iformat = luaL_checkinteger(L, 2);
    index = 4;
  }

  height = luagl_get_array2uc(L, index, &pixels, &width);
  if (height != -1)
  {
    glTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1),
                 iformat, width/iformat, height, 0, e, GL_UNSIGNED_BYTE, pixels);
  }
  else  /* if not 2D, get 1D */
  {
    width = luagl_get_arrayuc(L, index, &pixels);
    glTexImage1D(GL_TEXTURE_1D, luaL_checkinteger(L, 1),
                 iformat, width/iformat, 0, e, GL_UNSIGNED_BYTE, pixels);
  }
  LUAGL_DELETE_ARRAY(pixels);
  return 0;
}

/* TexImage2D(level, depth, width, height, border, format, type, pixels) -> none*/
static int luagl_tex_image_2d(lua_State *L)
{
  glTexImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1),
              luaL_checkinteger(L, 2), (GLsizei)luaL_checkinteger(L, 3), 
              (GLsizei)luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), 
              luagl_get_gl_enum(L, 6), luagl_get_gl_enum(L, 7), luagl_checkuserdata(L, 8));
  return 0;
}

/* TexImage1D(level, depth, width, border, format, type, pixels) -> none*/
static int luagl_tex_image_1d(lua_State *L)
{
  glTexImage1D(GL_TEXTURE_1D, luaL_checkinteger(L, 1),
               luaL_checkinteger(L, 2), (GLsizei)luaL_checkinteger(L, 3), 
               luaL_checkinteger(L, 4), luagl_get_gl_enum(L, 5), 
               luagl_get_gl_enum(L, 6), luagl_checkuserdata(L, 7));
  return 0;
}

/*TexSubImage (level, format, pixels, xoffset) -> none
  TexSubImage (level, format, pixels, xoffset, yoffset) -> none*/
static int luagl_tex_sub_image(lua_State *L)
{
  GLenum format;
  GLfloat *pixels;
  GLsizei width, height;
  int depth;

  format = luagl_get_gl_enum(L, 2);
  depth = luagl_get_depth(format);
  if (depth == 0)
    luaL_argerror(L, 2, "unknown format");

  height = luagl_get_array2f(L, 3, &pixels, &width);
  if(height != -1)
  {
    glTexSubImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
                    luaL_checkinteger(L, 5), width/depth, height, format, GL_FLOAT, pixels);
  }
  else  /* if not 2D, get 1D */
  {
    width = luagl_get_arrayf(L, 3, &pixels);
    glTexSubImage1D(GL_TEXTURE_1D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 4),
                    width/depth, format, GL_FLOAT, pixels);
  }

  LUAGL_DELETE_ARRAY(pixels);
  return 0;
}

/* TexSubImage2D (level, xoffset, yoffset, width, height, format, type, pixels) -> none*/
static int luagl_tex_sub_image_2d(lua_State *L)
{
  glTexSubImage2D(GL_TEXTURE_2D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
                  luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5), 
                  luagl_get_gl_enum(L, 6), luagl_get_gl_enum(L, 7), luagl_checkuserdata(L, 8));
  return 0;
}

/* TexSubImage1D (level, xoffset, width, format, type, pixels) -> none*/
static int luagl_tex_sub_image_1d(lua_State *L)
{
  glTexSubImage1D(GL_TEXTURE_1D, luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
                  luaL_checkinteger(L, 3), luagl_get_gl_enum(L, 4), 
                  luagl_get_gl_enum(L, 5), luagl_checkuserdata(L, 6));
  return 0;
}

/*TexParameter (target, pname, param) -> none
TexParameter (target, pname, paramsArray) -> none*/
static int luagl_tex_parameter(lua_State *L)
{
  GLenum e1, e2;

  e1 = luagl_get_gl_enum(L, 1);
  e2 = luagl_get_gl_enum(L, 2);

  if(lua_istable(L, 3))
  {
    GLfloat *param;
    luagl_get_arrayf(L, 3, &param);

    glTexParameterfv(e1, e2, (GLfloat *)param);

    LUAGL_DELETE_ARRAY(param);
  }
  else if(lua_isnumber(L, 3))
    glTexParameterf(e1, e2, (GLfloat)luaL_checknumber(L, 3));
  else 
    glTexParameteri(e1, e2, luagl_get_gl_enum(L, 3));

  return 0;
}

/*Translate (x, y, z) -> none*/
static int luagl_translate(lua_State *L)
{
  glTranslated(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  return 0;
}

/*Vertex (x, y, [z, w]) -> none
  Vertex (v) -> none*/
static int luagl_vertex(lua_State *L)
{
  int index;
  int num_args = lua_gettop(L);

  GLdouble *v;

  if(lua_istable(L, 1))
    num_args = luagl_get_arrayd(L, 1, &v);
  else
  {
    v = LUAGL_NEW_ARRAY(GLdouble, num_args);

    for(index = 0; index < num_args; index++)
    {
      v[index] = luaL_checknumber(L, index+1);
    }
  }

  if (num_args < 2)
    luaL_error(L, "invalid number of arguments");

  switch(num_args)
  {
  case 2:  glVertex2dv((GLdouble *)v);  break;
  case 3:  glVertex3dv((GLdouble *)v);  break;
  case 4:  glVertex4dv((GLdouble *)v);  break;
  }

  LUAGL_DELETE_ARRAY(v);

  return 0;
}

/*VertexPointer (vertexArray) -> none*/
static int luagl_vertex_pointer(lua_State *L)
{
  GLint size;
  static GLdouble *parray = NULL;

  LUAGL_DELETE_ARRAY(parray);

  if(lua_isnil(L,1))
    return 0;

  if (lua_isnumber(L, 2))
  {
    size = luaL_checkinteger(L, 2);
    luagl_get_arrayd(L, 1, &parray);
  }
  else 
  {
    int h = luagl_get_array2d(L, 1, &parray, &size);
    if (h==-1)
      luaL_argerror(L, 1, "must be a table of tables");
  }

  glVertexPointer(size, GL_DOUBLE, 0, parray);

  return 0;
}

/*Viewport (x, y, width, height) -> none*/
static int luagl_viewport(lua_State *L)
{
  glViewport(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), 
             luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  return 0;
}

static const luaL_reg luagl_lib[] = {
  {"Accum", luagl_accum},
  {"AlphaFunc", luagl_alpha_func},
  {"AreTexturesResident", luagl_are_textures_resident},
  {"ArrayElement", luagl_array_element},
  {"Begin", luagl_begin},
  {"BindTexture", luagl_bind_texture},
  {"Bitmap", luagl_bitmap},
  {"BitmapRaw", luagl_bitmap_raw},
  {"BlendFunc", luagl_blend_func},
  {"CallList", luagl_call_list},
  {"CallLists", luagl_call_lists},
  {"Clear", luagl_clear},
  {"ClearAccum", luagl_clear_accum},
  {"ClearColor", luagl_clear_color},
  {"ClearDepth", luagl_clear_depth},
  {"ClearIndex", luagl_clear_index},
  {"ClearStencil", luagl_clear_stencil},
  {"ClipPlane", luagl_clip_plane},
  {"Color", luagl_color},
  {"ColorMask", luagl_color_mask},
  {"ColorMaterial", luagl_color_material},
  {"ColorPointer", luagl_color_pointer},
  {"CopyPixels", luagl_copy_pixels},
  {"CopyTexImage", luagl_copy_tex_image},
  {"CopyTexSubImage", luagl_copy_tex_sub_image},
  {"CullFace",luagl_cull_face},
  {"DeleteLists",luagl_delete_lists},
  {"DeleteTextures",luagl_delete_textures},
  {"DepthFunc",luagl_depth_func},
  {"DepthMask",luagl_depth_mask},
  {"DepthRange",luagl_depth_range},
  {"Disable",luagl_disable},
  {"DisableClientState",luagl_disable_client_state},
  {"DrawArrays",luagl_draw_arrays},
  {"DrawBuffer",luagl_draw_buffer},
  {"DrawElements", luagl_draw_elements},
  {"DrawPixels", luagl_draw_pixels},
  {"DrawPixelsRaw", luagl_draw_pixels_raw},
  {"EdgeFlag", luagl_edge_flag},
  {"EdgeFlagPointer", luagl_edge_flag_pointer},
  {"Enable", luagl_enable},
  {"EnableClientState", luagl_enable_client_state},
  {"End", luagl_end},
  {"EndList", luagl_end_list},
  {"EvalCoord", luagl_eval_coord},
  {"EvalMesh", luagl_eval_mesh},
  {"EvalPoint", luagl_eval_point},
  {"FeedbackBuffer", luagl_feedback_buffer},
  {"Finish", luagl_finish},
  {"Flush", luagl_flush},
  {"Fog", luagl_fog},
  {"FrontFace", luagl_front_face},
  {"Frustum", luagl_frustum},
  {"GenLists", luagl_gen_lists},
  {"GenTextures", luagl_gen_textures},
  {"Get", luagl_get},
  {"GetArray", luagl_get_array},
  {"GetConst", luagl_get_const},
  {"GetClipPlane", luagl_get_clip_plane},
  {"GetError", luagl_get_error},
  {"GetLight", luagl_get_light},
  {"GetMap", luagl_get_map},
  {"GetMaterial", luagl_get_material},
  {"GetPixelMap", luagl_get_pixel_map},
  {"GetPointer", luagl_get_pointer},
  {"GetPolygonStipple", luagl_get_polygon_stipple},
  {"GetString", luagl_get_string},
  {"GetTexEnv", luagl_get_tex_env},
  {"GetTexGen", luagl_get_tex_gen},
  {"GetTexImage", luagl_get_tex_image},
  {"GetTexImageRaw", luagl_get_tex_image_raw},
  {"GetTexLevelParameter", luagl_get_tex_level_parameter},
  {"GetTexParameter", luagl_get_tex_parameter},
  {"Hint", luagl_hint},
  {"Index", luagl_index},
  {"IndexMask", luagl_index_mask},
  {"IndexPointer", luagl_index_pointer},
  {"InitNames", luagl_init_names},
  {"IsEnabled", luagl_is_enabled},
  {"IsList", luagl_is_list},
  {"IsTexture", luagl_is_texture},
  {"Light", luagl_light},
  {"LightModel", luagl_light_model},
  {"LineStipple", luagl_line_stipple},
  {"LineWidth", luagl_line_width},
  {"ListBase", luagl_list_base},
  {"LoadIdentity", luagl_load_identity},
  {"LoadMatrix", luagl_load_matrix},
  {"LoadName", luagl_load_name},
  {"LogicOp", luagl_logic_op},
  {"Map", luagl_map},
  {"MapGrid", luagl_map_grid},
  {"Material", luagl_material},
  {"MatrixMode", luagl_matrix_mode},
  {"MultMatrix", luagl_mult_matrix},
  {"NewList", luagl_new_list},
  {"Normal", luagl_normal},
  {"NormalPointer", luagl_normal_pointer},
  {"Ortho", luagl_ortho},
  {"PassThrough", luagl_pass_through},
  {"PixelMap", luagl_pixel_map},
  {"PixelStore", luagl_pixel_store},
  {"PixelTransfer", luagl_pixel_transfer},
  {"PixelZoom", luagl_pixel_zoom},
  {"PointSize", luagl_point_size},
  {"PolygonMode", luagl_polygon_mode},
  {"PolygonOffset", luagl_polygon_offset},
  {"PolygonStipple", luagl_polygon_stipple},
  {"PopAttrib", luagl_pop_attrib},
  {"PopClientAttrib", luagl_pop_client_attrib},
  {"PopMatrix", luagl_pop_matrix},
  {"PopName", luagl_pop_name},
  {"PrioritizeTextures", luagl_prioritize_textures},
  {"PushAttrib", luagl_push_attrib},
  {"PushClientAttrib", luagl_push_client_attrib},
  {"PushMatrix", luagl_push_matrix},
  {"PushName", luagl_push_name},
  {"RasterPos", luagl_raster_pos},
  {"ReadBuffer", luagl_read_buffer},
  {"ReadPixels", luagl_read_pixels},
  {"ReadPixelsRaw", luagl_read_pixels_raw},
  {"Rect", luagl_rect},
  {"RenderMode", luagl_render_mode},
  {"Rotate", luagl_rotate},
  {"Scale", luagl_scale},
  {"Scissor", luagl_scissor},
  {"SelectBuffer", luagl_select_buffer},
  {"GetSelectBuffer", luagl_get_select_buffer},
  {"FreeSelectBuffer", luagl_free_select_buffer},
  {"NewData", luagl_new_data},
  {"FreeData", luagl_free_data},
  {"ShadeModel", luagl_shade_model},
  {"StencilFunc", luagl_stencil_func},
  {"StencilMask", luagl_stencil_mask},
  {"StencilOp", luagl_stencil_op},
  {"TexCoord", luagl_tex_coord},
  {"TexCoordPointer", luagl_tex_coord_pointer},
  {"TexEnv", luagl_tex_env},
  {"TexGen", luagl_tex_gen},
  {"TexImage", luagl_tex_image},
  {"TexImage1D", luagl_tex_image_1d},
  {"TexImage2D", luagl_tex_image_2d},
  {"TexSubImage", luagl_tex_sub_image},
  {"TexSubImage1D", luagl_tex_sub_image_1d},
  {"TexSubImage2D", luagl_tex_sub_image_2d},
  {"TexParameter", luagl_tex_parameter},
  {"Translate", luagl_translate},
  {"Vertex", luagl_vertex},
  {"VertexPointer", luagl_vertex_pointer},
  {"Viewport", luagl_viewport},
  {NULL, NULL}
};

int luaopen_gl_luagl(lua_State *L) 
{
  luaL_register(L, "gl", luagl_lib);

  luagl_initconst(L, luagl_const);

  lua_pushstring(L, "_VERSION");
  lua_pushstring(L, LUAGL_VERSION);
  lua_settable(L,-3);

  return 1;
}
