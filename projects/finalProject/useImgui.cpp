//imguiͷ�ļ�
#include "useImgui.h"

ImguiSettings::ImguiSettings() {
	
}

void ImguiSettings::initImgui(GLFWwindow* _window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();
	//����imgui

}

void ImguiSettings::useImgui()
{


//

ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

const auto flags =
ImGuiWindowFlags_AlwaysAutoResize |    //��֡����ui���ݲ�ʵʱ����
ImGuiWindowFlags_NoSavedSettings;      //�������κη���

if (!ImGui::Begin("Control Panel", nullptr, flags)) {      //��չ������������Ŀ��Ӧ��flags��һ������ü��������κη���
	ImGui::End();
}
else {
	//shaders
	ImGui::Text("Shader");                  //�����ı���ʾ
	ImGui::Separator();                     //�����ָ���
	ImGui::NewLine();                       //��������
	ImGui::RadioButton("DefaultShader", (int*)&_shadermode, (int)(0));                //����RadioButton������ֻ��ѡ��һ���������л�shader
	ImGui::NewLine();                       //����
	ImGui::RadioButton("Shadowing", (int*)&_shadermode, (int)(1));
	ImGui::NewLine();                       //����
	ImGui::RadioButton("Shader3", (int*)&_shadermode, (int)(2));
	ImGui::NewLine();                       //����

	//cameras
	ImGui::Text("Camera");
	ImGui::Separator();
	ImGui::NewLine();
	//use this button to select camera
	_useNextCamera = ImGui::IsItemClicked(ImGui::Button("Next Camera"));
	ImGui::NewLine();

	//spot Light
	ImGui::Text("CharacterLight");                   //�ı���ʾ
	ImGui::Separator();                     //�ָ���
	ImGui::ColorEdit3("color", (float*)&_spotLightColor);                          //����3��float���ͱ�����RGBģʽ������ɫ
	ImGui::NewLine();                       //����
	ImGui::SliderFloat("intensity", &_spotLightintensity, 0.0f, 1.0f);                       //��������float���ͱ���������ͼȨ��
	ImGui::NewLine();                       //����
	ImGui::SliderFloat("angle", &_spotLightdegree, 5.0f, 90.0f);                       //��������float���ͱ���������ͼȨ��
	ImGui::NewLine();                       //����
	ImGui::SliderFloat("attenuation", &_spotLightattenuation, 1.0f, 20.0f);
	ImGui::NewLine();

	//character Material
	ImGui::Text("CharacterMaterial");                   //�ı���ʾ
	ImGui::Separator();                     //�ָ���
	ImGui::NewLine();                       //����
	ImGui::ColorEdit3("diffuse", (float*)_diffColor);
	ImGui::NewLine();
	ImGui::ColorEdit3("specular", (float*)_specular);
	ImGui::NewLine();
	ImGui::SliderFloat("gloss", &_gloss, 1.0f, 20.0f);
	ImGui::NewLine();                       //����
	ImGui::SliderFloat("textureBlend", &_blend, 0.0f, 1.0f);                       //������������float���ͱ���������ͼȨ��
	ImGui::NewLine();                       //����
	ImGui::Checkbox("Checkpoint", &_check);                               //������ʾȷ�����Ĵ������������ͱ�����������չʾ
	ImGui::NewLine();                       //����
	//export Mesh
	ImGui::Text("Mesh Export");
	ImGui::Separator();
	ImGui::NewLine();
	_exportMesh = ImGui::IsItemClicked(ImGui::Button("Export Mesh"));
	ImGui::End();                           //ui����


}

ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//����ui
}