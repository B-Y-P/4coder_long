#include "4coder_qol_odin_parser.cpp"

internal F4_LANGUAGE_INDEXFILE(Long_Odin_IndexFile){
    Arena arena = make_arena_system(KB(16));
    Code_Index_File *index = push_array_zero(&arena, Code_Index_File, 1);
    index->buffer = ctx->file->buffer;

    Generic_Parse_State state = {};
    generic_parse_init(ctx->app, &arena, ctx->string, &ctx->tokens, default_comment_index, &state);
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
    
    code_index_lock();
    code_index_set_file(index->buffer, arena, index);
    code_index_unlock();
    buffer_clear_layout_cache(ctx->app, index->buffer);
}

function Managed_ID
qol_get_token_color_odin(Token *token){
  switch (token->sub_kind){
    case TokenOdinKind_Directive:         return fleury_color_index_macro;
    case TokenOdinKind_Parametric:        return fleury_color_syntax_crap;
    case TokenOdinKind_LiteralCharacter:  return defcolor_char_constant;
  }
  switch (token->kind){
    case TokenBaseKind_Preprocessor:      return defcolor_preproc;               
    case TokenBaseKind_Keyword:           return defcolor_keyword;               
    case TokenBaseKind_Comment:           return defcolor_comment;               
    case TokenBaseKind_LiteralString:     return defcolor_str_constant;          
    case TokenBaseKind_LiteralInteger:    return defcolor_int_constant;          
    case TokenBaseKind_LiteralFloat:      return defcolor_float_constant;        
    case qol_TokenKind_Control:           return fleury_color_index_macro;
    case qol_TokenKind_Primitive:         return fleury_color_index_product_type;
    case qol_TokenKind_Struct:            return defcolor_keyword;
  }
  return 0;
}

internal F4_LANGUAGE_HIGHLIGHT(Long_Odin_Highlight){
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    i64 first_index = token_index_from_pos(array, visible_range.first);
    Token_Iterator_Array it = token_iterator_index(0, array, first_index);

    for(;;)
    {
        Token *token = token_it_read(&it);
        if(!token || token->pos >= visible_range.one_past_last)
        {
            break;
        }

        Managed_ID id = qol_get_token_color_odin(token);
        if(id != 0)
        {
            paint_text_color(app, text_layout_id, Ii64(token), fcolor_resolve(fcolor_id(id)));
        }

        if(!token_it_inc_all(&it))
        {
            break;
        }
    }
}

internal F4_LANGUAGE_POSCONTEXT(Long_Odin_PosContext){ return NULL; }