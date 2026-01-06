#include "4coder_qol_odin_parser.cpp"

internal F4_LANGUAGE_INDEXFILE(Long_Odin_IndexFile){
    Scratch_Block scratch(ctx->app);
    Code_Index_File *index = push_array_zero(scratch, Code_Index_File, 1);
    index->buffer = ctx->file->buffer;

    Generic_Parse_State state = {};
    generic_parse_init(ctx->app, &ctx->file->arena, ctx->string, &ctx->tokens, default_comment_index, &state);
    odin_parse_full_input_breaks(index, &state, max_i32);
    F4_Index_NoteKind lut[CodeIndexNote_COUNT] = { 
        /*[CodeIndexNote_Type    ] = */ F4_Index_NoteKind_Type,
        /*[CodeIndexNote_Function] = */ F4_Index_NoteKind_Function,
        /*[CodeIndexNote_Macro   ] = */ F4_Index_NoteKind_Macro,
        /*[CodeIndexNote_Enum    ] = */ F4_Index_NoteKind_Constant,
        /*[CodeIndexNote_Global  ] = */ F4_Index_NoteKind_Decl
    };
    
    for (i32 i = 0; i < index->note_array.count; i += 1){
        Code_Index_Note *note = index->note_array.ptrs[i];
        F4_Index_MakeNote(ctx, note->pos, lut[note->note_kind], 0);
    }
}

internal F4_LANGUAGE_POSCONTEXT(Long_Odin_PosContext){ return NULL; }
internal F4_LANGUAGE_HIGHLIGHT(Long_Odin_Highlight){ ; }
