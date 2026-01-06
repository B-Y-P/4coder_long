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

function FColor
qol_get_token_color_odin(Token token){
  Managed_ID color = 0;
  switch (token.kind){
    case TokenBaseKind_Preprocessor:   { color = defcolor_preproc;        }break;
    case TokenBaseKind_Keyword:        { color = defcolor_keyword;        }break;
    case TokenBaseKind_Comment:        { color = defcolor_comment;        }break;
    case TokenBaseKind_LiteralString:  { color = defcolor_str_constant;   }break;
    case TokenBaseKind_LiteralInteger: { color = defcolor_int_constant;   }break;
    case TokenBaseKind_LiteralFloat:   { color = defcolor_float_constant; }break;

    //case TokenBaseKind_Operator:{ color = fleury_color_operators; }break;
    //case TokenBaseKind_ScopeOpen:
    //case TokenBaseKind_ScopeClose:
    //case TokenBaseKind_ParentheticalOpen:
    //case TokenBaseKind_ParentheticalClose:
    //case TokenBaseKind_StatementClose:{ color = fleury_color_syntax_crap; } break;

    case qol_TokenKind_Control:{ color = fleury_color_index_macro; }break;
    case qol_TokenKind_Primitive:{ color = fleury_color_index_product_type; }break;
    case qol_TokenKind_Struct:{ color = defcolor_keyword; }break;
  }
  // specifics override generals
  switch (token.sub_kind){
    case TokenOdinKind_Directive: { color = fleury_color_index_macro; }break;
    case TokenOdinKind_Parametric: { color = fleury_color_syntax_crap; }break;
    case TokenOdinKind_LiteralCharacter: { color = defcolor_char_constant; }break;
  }
  return fcolor_id(color);
}

 // void name(Application_Links *app, Text_Layout_ID text_layout_id, Token_Array *array, Color_Table color_table)
internal F4_LANGUAGE_HIGHLIGHT(Long_Odin_Highlight){ ; 
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
        FColor fcolor = qol_get_token_color_odin(*token);
        if (fcolor.id != 0){
            paint_text_color(app, text_layout_id, Ii64(token), fcolor_resolve(fcolor));
        }
        
        if(!token_it_inc_all(&it))
        {
            break;
        }
    }
}
