#pragma once

#include "pch.h"
#include "GLCanvas.h"
#include "Game.h"

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
	Game^ game;

public:
	MainForm();

protected:
	~MainForm();
private: System::Windows::Forms::Button^ btnSomething;
protected:


private: System::Windows::Forms::Control^ glCanvasPlaceholder;
private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	void InitializeComponent(void)
	{
		this->btnSomething = (gcnew System::Windows::Forms::Button());
		this->glCanvasPlaceholder = (gcnew System::Windows::Forms::Control());
		this->SuspendLayout();
		// 
		// btnSomething
		// 
		this->btnSomething->Location = System::Drawing::Point(13, 13);
		this->btnSomething->Name = L"btnSomething";
		this->btnSomething->Size = System::Drawing::Size(89, 23);
		this->btnSomething->TabIndex = 0;
		this->btnSomething->Text = L"Do something";
		this->btnSomething->UseVisualStyleBackColor = true;
		this->btnSomething->Click += gcnew System::EventHandler(this, &MainForm::btnSomething_Click);
		// 
		// glCanvasPlaceholder
		// 
		this->glCanvasPlaceholder->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->glCanvasPlaceholder->BackColor = System::Drawing::Color::DarkOrchid;
		this->glCanvasPlaceholder->Location = System::Drawing::Point(13, 42);
		this->glCanvasPlaceholder->Name = L"glCanvasPlaceholder";
		this->glCanvasPlaceholder->Size = System::Drawing::Size(259, 207);
		this->glCanvasPlaceholder->TabIndex = 1;
		// 
		// MainForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(284, 261);
		this->Controls->Add(this->glCanvasPlaceholder);
		this->Controls->Add(this->btnSomething);
		this->Name = L"MainForm";
		this->Text = L"MainForm";
		this->ResumeLayout(false);

	}
#pragma endregion
private: System::Void btnSomething_Click(System::Object^ sender, System::EventArgs^ e);
};