//===========================================================================
//!	@file	Text.cpp
//!	@brief	�e�L�X�g
//===========================================================================
#include"Text.h"

namespace gpu {

   


    //---------------------------------------------------------------------------
    //! �e�L�X�g����
    //! @parm [in] x	 x���W
    //! @parm [in] y	 y���W
    //! @parm [in] scale �e�L�X�g�T�C�Y
    //! @parm [in] text  �e�L�X�g
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
    //! ���̃e�L�X�g���W��ݒ�
    //! @parm [in] x	 x���W
    //! @parm [in] y	 y���W
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



