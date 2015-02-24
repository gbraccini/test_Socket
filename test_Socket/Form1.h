#pragma once
 
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <msclr\marshal.h>
#include <map>
#include <string>
 
#define DEFAULT_PORT "5027"
#define WM_SOCKET WM_USER + 1
// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
	
namespace test_Socket {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		SOCKET ListenSocket;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(12, 30);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(694, 76);
			this->textBox1->TabIndex = 1;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(14, 135);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(694, 76);
			this->textBox2->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Client 1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 119);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(42, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Client 2";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(720, 294);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
	
 
	 
 
 

			 }

protected: virtual void Form1::WndProc(System::Windows::Forms::Message %m) override  
             {       
				 static std::map <SOCKET, std::string> client_list;

				 SOCKET Accept;
				 char recvbuf[1024];
						 
				 std::string client;
                 if(m.Msg == WM_SOCKET)  
                 {
					   // Determine whether an error occurred on the

					// socket by using the WSAGETSELECTERROR() macro

					if (WSAGETSELECTERROR( DWORD_PTR(m.LParam.ToPointer()) ) )

					{
						  // Display the error and close the socket

						 closesocket( (SOCKET) (m.WParam.ToPointer()));

						 
					}
					else
					{
						 // Determine what event occurred on the socket
					
						switch(WSAGETSELECTEVENT(DWORD_PTR(m.LParam.ToPointer())))

						{

							case FD_ACCEPT:

								// Accept an incoming connection

								Accept = accept(SOCKET(m.WParam.ToPointer()), NULL, NULL);

								 
								char buffer[64];
								
								itoa(  client_list.size() + 1, buffer, 10 );
								client_list[Accept] = buffer;

								// Prepare accepted socket for read, write, and close notification

								WSAAsyncSelect(Accept, HWND(m.HWnd.ToPointer()), WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

								break;

							case FD_READ:

								// Receive data from the socket in wParam
						
								// reveice until the client shutdown the connection
								 
									memset( recvbuf, 0, 1024 );
									recv((SOCKET)m.WParam.ToPointer(), recvbuf, 1024, 0);
									
									client = client_list[(SOCKET)m.WParam.ToPointer()];
									if( client == std::string("1") )
									{
										 this->textBox1->Text += gcnew  String(recvbuf) ;
											 
									}
									else
									{
										 this->textBox2->Text += gcnew  String(recvbuf) ;
									}

									
									 

								break;

							case FD_WRITE:

								// The socket in wParam is ready for sending data

								break;

							case FD_CLOSE:

								// The connection is now closed

								closesocket((SOCKET)m.WParam.ToPointer());

								break;

						}

					}
                   
                 }  
                 System::Windows::Forms::Form::WndProc(m);  
             } 

	private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
			 {
				closesocket(ListenSocket);
				WSACleanup();
			 }

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
			 			 
				WSADATA wsaData;
 
				// Initialize Winsock
				int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
				if(iResult != 0)
				{
					MessageBox::Show( "WSAStartup failed." );
					return;
				}
 
				struct addrinfo	*result = NULL,
				hints;
 
				ZeroMemory(&hints, sizeof(hints));
				hints.ai_family = AF_INET;	// Internet address family is unspecified so that either an IPv6 or IPv4 address can be returned
				hints.ai_socktype = SOCK_STREAM;	// Requests the socket type to be a stream socket for the TCP protocol
				hints.ai_protocol = IPPROTO_TCP;
				hints.ai_flags = AI_PASSIVE;
 
				// Resolve the local address and port to be used by the server
				iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
				if (iResult != 0)
				{
					MessageBox::Show("getaddrinfo failed.");
					WSACleanup();
					return;
				}
 
				ListenSocket = INVALID_SOCKET;
 
				// Create a SOCKET for the server to listen for client connections
				ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
 
				if (ListenSocket == INVALID_SOCKET)
				{
					MessageBox::Show("Error at socket()");
					freeaddrinfo(result);
					WSACleanup();
					return;
				}
 
				// Setup the TCP listening socket
				iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
 
				if (iResult == SOCKET_ERROR)
				{
					MessageBox::Show("bind failed:");
					freeaddrinfo(result);
					closesocket(ListenSocket);
					WSACleanup();
					return;
				}
 
				freeaddrinfo(result);
 
				 // Set up window message notification on

				// the new socket using the WM_SOCKET define above
				HWND hWnd;
				hWnd = static_cast<HWND>(Handle.ToPointer());

				WSAAsyncSelect(ListenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

				// To listen on a socket
				if ( listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
				{
					MessageBox::Show("listen failed: %d\n");
					closesocket(ListenSocket);
					WSACleanup();
					return;
				}
		     }
};

	 
}

