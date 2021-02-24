//===========================================================================
//!	@file	Text.cpp
//!	@brief	テキスト
//===========================================================================
#include"Text.h"

namespace gpu {

   


    //---------------------------------------------------------------------------
    //! テキスト生成
    //! @parm [in] x	 x座標
    //! @parm [in] y	 y座標
    //! @parm [in] scale テキストサイズ
    //! @parm [in] text  テキスト
    //---------------------------------------------------------------------------
    void createText(f32 x, f32 y, f32 scale, const char* text,...)
	{
        ImGui::SetNextWindowPos({ x, y });
        ImGuiWindowFlags window_flags = 0;
        window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin(text, (bool*)0, window_flags);
        {
            ImGui::GetWindowPos();
            ImGui::SetWindowFontScale(scale);
            ImGui::Text(text);
        }
        ImGui::End();
	}
    
    //---------------------------------------------------------------------------
    //! 次のテキスト座標を設定
    //! @parm [in] x	 x座標
    //! @parm [in] y	 y座標
    //---------------------------------------------------------------------------
    void nextTextPos(f32 x, f32 y)
    {
        ImGui::SetNextWindowPos({ x, y });
        ImGuiWindowFlags window_flags = 0;
        window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    }


}



