#pragma once
#include "edge_cluster_graph.h"
#include "draw_util.h"
#include "lpp_algorithm\sa.h"
#include "lpp_algorithm\sa_enhanced.h"
#include <fstream>
#include <cstdlib>
using namespace std;

namespace graph {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// display 摘要
	/// </summary>
	public ref class display : public System::Windows::Forms::Form
	{
	public:
		display()
		{
			InitializeComponent();
			p_graph = nullptr;
			//
			//TODO:  在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~display()
		{
			if (components)
			{
				delete components;
			}
		}
	private: EC_Graph *p_graph;
	private: Draw_Util *p_du;
	private: int draw_index;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button10;



	protected:

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Location = System::Drawing::Point(12, 3);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1200, 1200);
			this->panel1->TabIndex = 0;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &display::panel1_Paint);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(1267, 235);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(124, 46);
			this->button1->TabIndex = 1;
			this->button1->Text = L"绘制原图";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &display::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(1267, 305);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(124, 46);
			this->button2->TabIndex = 1;
			this->button2->Text = L"绘边簇图";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &display::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(1267, 539);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(124, 46);
			this->button3->TabIndex = 1;
			this->button3->Text = L"放大";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &display::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(1267, 611);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(124, 46);
			this->button4->TabIndex = 1;
			this->button4->Text = L"缩小";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &display::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(1302, 699);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(46, 42);
			this->button5->TabIndex = 2;
			this->button5->Text = L"上";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &display::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(1302, 795);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(46, 42);
			this->button6->TabIndex = 2;
			this->button6->Text = L"下";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &display::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(1254, 746);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(46, 42);
			this->button7->TabIndex = 2;
			this->button7->Text = L"左";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &display::button7_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(1348, 746);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(46, 42);
			this->button8->TabIndex = 2;
			this->button8->Text = L"右";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &display::button8_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(1267, 379);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(138, 29);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"启用视距";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"古登堡", L"旧金山" });
			this->comboBox1->Location = System::Drawing::Point(1269, 79);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 32);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &display::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(1264, 42);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(112, 25);
			this->label1->TabIndex = 5;
			this->label1->Text = L"选择地图";
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(1267, 928);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(124, 46);
			this->button10->TabIndex = 7;
			this->button10->Text = L"执行算法";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &display::button10_Click);
			// 
			// display
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 24);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(1425, 1255);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->panel1);
			this->Name = L"display";
			this->Text = L"display";
			this->Load += gcnew System::EventHandler(this, &display::display_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private:void update_ec_view()
		{
			Bitmap ^bmp = gcnew Bitmap(p_du->get_paint_sizex(), p_du->get_paint_sizey());
			Graphics ^mg = Graphics::FromImage(bmp);
			Color color = BackColor; //获取背景颜色
			mg->Clear(color);  //清空；
			Pen ^ red_pen = gcnew Pen(Color::Red, 1.0f);
			red_pen->LineJoin = Drawing2D::LineJoin::Miter;

			int edge_cnt = p_graph->get_edges().size();
			vector<EC_Edge*> edges = p_graph->get_edges();
			double x1, x2, y1, y2;
			EC_Node *p_node1, *p_node2;
			for (int i = 0; i < edge_cnt; i++) 
			{
				p_node1 = edges[i]->getEc_node1();
				p_node2 = edges[i]->getEc_node2();
				x1 = p_node1->get_x();
				y1 = p_node1->get_y();
				x2 = p_node2->get_x();
				y2 = p_node2->get_y();
				p_du->convert_to_draw_pos(x1, y1);
				p_du->convert_to_draw_pos(x2, y2);
				if ((x1<0 && x2<0) || (x1>p_du->get_paint_sizex() && x2>p_du->get_paint_sizex())) continue;
				if ((y1<0 && y2<0) || (y1>p_du->get_paint_sizey() && y2>p_du->get_paint_sizey())) continue;
				//if(edges[i]->getEdgeClass()<5)
				//mg->DrawLine(pen_list[edges[i]->getEdgeClass()], (float)x1, (float)y1, (float)x2, (float)y2);
				//else
				mg->DrawLine(red_pen, (float)x1, (float)y1, (float)x2, (float)y2);
			}
			this->panel1->CreateGraphics()->DrawImage(bmp, 0, 0);
		}
	private:void update_view() // 绘图需要消耗较多时间，但绘图不是必要的
	{
		Bitmap ^bmp = gcnew Bitmap(p_du->get_paint_sizex(), p_du->get_paint_sizey());
		Graphics ^mg = Graphics::FromImage(bmp);
		Color color = BackColor; //获取背景颜色
		mg->Clear(color);  //清空；
		Pen ^ red_pen = gcnew Pen(Color::Red, 2.0f);
		Pen ^ yellow_pen = gcnew Pen(Color::Orange, 1.8f);
		Pen ^ blue_pen = gcnew Pen(Color::Blue, 1.5f);
		Pen ^ green_pen = gcnew Pen(Color::Green, 1.0f);
		Pen ^ dark_gray_pen = gcnew Pen(Color::DarkGray, 0.5f);
		Pen ^ gray_pen = gcnew Pen(Color::Gray, 0.5f);
		Pen ^ light_gray_pen = gcnew Pen(Color::LightGray, 0.5f);
		cli::array<Pen^>^ pen_list = { red_pen,yellow_pen,blue_pen,green_pen,dark_gray_pen,gray_pen,light_gray_pen };
		int edge_cnt = p_graph->p_graph->getEdges().size();
		vector<Edge*> edges = p_graph->p_graph->getEdges();
		double x1, x2, y1, y2;
		Node *p_node1, *p_node2;
		bool need_scaling = this->checkBox1->Checked;
		for (int i = 0; i < edge_cnt; i++) {
			p_node1 = edges[i]->getNode1();
			p_node2 = edges[i]->getNode2();
			x1 = p_node1->getX();
			y1 = p_node1->getY();
			x2 = p_node2->getX();
			y2 = p_node2->getY();
			p_du->convert_to_draw_pos(x1, y1);
			p_du->convert_to_draw_pos(x2, y2);
			if ((x1<0 && x2<0) || (x1>p_du->get_paint_sizex() && x2>p_du->get_paint_sizex())) continue;
			if ((y1<0 && y2<0) || (y1>p_du->get_paint_sizey() && y2>p_du->get_paint_sizey())) continue;
			if ((edges[i]->getEdgeClass() < p_du->get_zoom() + 1 || !need_scaling)) {
				//绘制边
				mg->DrawLine(pen_list[edges[i]->getEdgeClass()], (float)x1, (float)y1, (float)x2, (float)y2);
				if (x1 == x2) continue;
				//绘制边上的兴趣点
				int poi_size = edges[i]->get_pois().size();
				for (int j = 1; j <= poi_size; j++) {
					float x, y;
					x = x1 + ((float)j / (poi_size + 1)) * (x2-x1);
					y = y1 + (x-x1)*(y2 - y1) / (x2 - x1);
					if (edges[i]->get_pois()[j - 1]->get_type()==Semantic_type::hospital) {
						mg->DrawRectangle(red_pen, x, y, 5.0, 5.0);
						
					}
					else{
						mg->DrawRectangle(green_pen, x, y, 5.0, 5.0);
					}
					
				}
				// 绘制边上的lbs用户
				int user_cnt = edges[i]->get_users().size();
				for (int j = 1; j <= user_cnt; j++) {
					float x, y;
					x = x1 + ((float)j / (user_cnt + 2)) * (x2 - x1);
					y = y1 + (x - x1)*(y2 - y1) / (x2 - x1);
					mg->DrawEllipse(blue_pen, x, y, 1.5, 1.5);
				}
			}
		}
		this->panel1->CreateGraphics()->DrawImage(bmp, 0, 0);
		
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		double x_min = p_graph->p_graph->get_xmin();
		double x_max = p_graph->p_graph->get_xmax();
		double y_min = p_graph->p_graph->get_ymin();
		double y_max = p_graph->p_graph->get_ymax();
		p_du = new Draw_Util(x_min, x_max, y_min, y_max, this->panel1->Size.Width, this->panel1->Size.Height, 20);
		update_view();
		draw_index = 1;
		this->button3->Enabled = true;
		this->button4->Enabled = false;
	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		double x_min = p_graph->p_graph->get_xmin();
		double x_max = p_graph->p_graph->get_xmax();
		double y_min = p_graph->p_graph->get_ymin();
		double y_max = p_graph->p_graph->get_ymax();
		p_du = new Draw_Util(x_min, x_max, y_min, y_max, this->panel1->Size.Width, this->panel1->Size.Height, 20);
		update_ec_view();
		draw_index = 2;
		this->button3->Enabled = true;
		this->button4->Enabled = false;
	}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->zoom_plus();
	if (p_du->get_zoom() > 6) this->button3->Enabled = false;
	this->button4->Enabled = true;
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->zoom_sub();
	if (p_du->get_zoom() < 2) this->button4->Enabled = false;
	this->button3->Enabled = true;
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->top_move();
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->buttom_move();
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->left_move();
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
	p_du->right_move();
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	ifstream if_nodes, if_edges;
	if (this->comboBox1->SelectedIndex == 0) {
		if_nodes.open("map_data\\oldenburgGen_node.txt");
		if_edges.open("map_data\\oldenburgGen_edge.txt");
	}
	else if (this->comboBox1->SelectedIndex == 1) {
		if_nodes.open("map_data\\sanfrancisco_node.txt");
		if_edges.open("map_data\\sanfrancisco_edge.txt");
	}
	this->button1->Text = "等待";
	this->button2->Text = "等待";
	this->button1->Enabled = false;
	this->button2->Enabled = false;
	if (this->p_graph != nullptr) {
		delete(this->p_graph);
	}
	EC_Graph *p_graph = new EC_Graph(if_nodes, if_edges);
	this->p_graph = p_graph;
	this->button1->Text = "绘制原图";
	this->button2->Text = "绘边簇图";
	this->button1->Enabled = true;
	this->button2->Enabled = true;
	if_nodes.close();
	if_edges.close();
	//触发绘制原图
	cout << p_graph->p_graph->get_pois().size() << endl;
	this->button1_Click(sender, e);
}
private: System::Void display_Load(System::Object^  sender, System::EventArgs^  e) {
	this->comboBox1->SelectedIndex = 0;
}
private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	//this->button1_Click(sender, e);
	if (draw_index == 1)
		update_view();
	else
		update_ec_view();
}
private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
	//执行算法
	//Lppa_sa *p_sa = new Lppa_sa(this->p_graph->p_graph);
	Lppa_sa_e *p_sa = new Lppa_sa_e(this->p_graph->p_graph);
	p_sa->lpp();
	delete p_sa;
}
};
}
