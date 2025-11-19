#pragma once

#include "pch.h"
#include "GLCanvas.h"
#include "RubyDung.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class MainForm : public System::Windows::Forms::Form
{
private:
	GLCanvas^ glCanvas;
	RubyDung^ game;

public:
	MainForm();

protected:
	~MainForm();

private: System::Windows::Forms::MenuStrip^ msMenu;
private: System::Windows::Forms::ToolStripMenuItem^ aboutToolStripMenuItem;
private: System::Windows::Forms::Control^ glCanvasPlaceholder;
private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	void InitializeComponent(void)
	{
		this->glCanvasPlaceholder = (gcnew System::Windows::Forms::Control());
		this->msMenu = (gcnew System::Windows::Forms::MenuStrip());
		this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->msMenu->SuspendLayout();
		this->SuspendLayout();
		// 
		// glCanvasPlaceholder
		// 
		this->glCanvasPlaceholder->BackColor = System::Drawing::Color::DarkOrchid;
		this->glCanvasPlaceholder->Dock = System::Windows::Forms::DockStyle::Fill;
		this->glCanvasPlaceholder->Location = System::Drawing::Point(0, 24);
		this->glCanvasPlaceholder->Name = L"glCanvasPlaceholder";
		this->glCanvasPlaceholder->Size = System::Drawing::Size(284, 237);
		this->glCanvasPlaceholder->TabIndex = 1;
		// 
		// msMenu
		// 
		this->msMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
		this->msMenu->Location = System::Drawing::Point(0, 0);
		this->msMenu->Name = L"msMenu";
		this->msMenu->Size = System::Drawing::Size(284, 24);
		this->msMenu->TabIndex = 2;
		this->msMenu->Text = L"menuStrip1";
		// 
		// aboutToolStripMenuItem
		// 
		this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
		this->aboutToolStripMenuItem->Size = System::Drawing::Size(65, 20);
		this->aboutToolStripMenuItem->Text = L"About";
		this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aboutToolStripMenuItem_Click);
		// 
		// MainForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(284, 261);
		this->Controls->Add(this->glCanvasPlaceholder);
		this->Controls->Add(this->msMenu);
		this->MainMenuStrip = this->msMenu;
		this->Name = L"MainForm";
		this->ShowIcon = false;
		this->Text = L"Dung but Ruby";
		this->msMenu->ResumeLayout(false);
		this->msMenu->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
#pragma endregion

private: System::Void aboutToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
};