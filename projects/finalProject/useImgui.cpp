//imgui头文件
#include "useImgui.h"

ImguiSettings::ImguiSettings() {
	
}

void ImguiSettings::initImgui(GLFWwindow* _window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init();
	//启用imgui

}

void ImguiSettings::useImgui()
{


//

ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

const auto flags =
ImGuiWindowFlags_AlwaysAutoResize |    //逐帧调整ui内容并实时反馈
ImGuiWindowFlags_NoSavedSettings;      //不进行任何反馈

if (!ImGui::Begin("Control Panel", nullptr, flags)) {      //不展开顶部下拉栏目则应用flags后一项的设置即不进行任何反馈
	ImGui::End();
}
else {
	//shaders
	ImGui::Text("Shader");                  //新增文本显示
	ImGui::Separator();                     //新增分割线
	ImGui::NewLine();                       //新增空行
	ImGui::RadioButton("DefaultShader", (int*)&_shadermode, (int)(0));                //所有RadioButton中能且只能选中一个，用于切换shader
	ImGui::NewLine();                       //空行
	ImGui::RadioButton("Shadowing", (int*)&_shadermode, (int)(1));
	ImGui::NewLine();                       //空行
	ImGui::RadioButton("Shader3", (int*)&_shadermode, (int)(2));
	ImGui::NewLine();                       //空行

	//cameras
	ImGui::Text("Camera");
	ImGui::Separator();
	ImGui::NewLine();
	//use this button to select camera
	_useNextCamera = ImGui::IsItemClicked(ImGui::Button("Next Camera"));
	ImGui::NewLine();

	//spot Light
	ImGui::Text("CharacterLight");                   //文本显示
	ImGui::Separator();                     //分割线
	ImGui::ColorEdit3("color", (float*)&_spotLightColor);                          //新增3个float类型变量以RGB模式调整颜色
	ImGui::NewLine();                       //空行
	ImGui::SliderFloat("intensity", &_spotLightintensity, 0.0f, 1.0f);                       //滑动调整float类型变量调节贴图权重
	ImGui::NewLine();                       //空行
	ImGui::SliderFloat("angle", &_spotLightdegree, 5.0f, 90.0f);                       //滑动调整float类型变量调节贴图权重
	ImGui::NewLine();                       //空行
	ImGui::SliderFloat("attenuation", &_spotLightattenuation, 1.0f, 20.0f);
	ImGui::NewLine();

	//character Material
	ImGui::Text("CharacterMaterial");                   //文本显示
	ImGui::Separator();                     //分割线
	ImGui::NewLine();                       //空行
	ImGui::ColorEdit3("diffuse", (float*)_diffColor);
	ImGui::NewLine();
	ImGui::ColorEdit3("specular", (float*)_specular);
	ImGui::NewLine();
	ImGui::SliderFloat("gloss", &_gloss, 1.0f, 20.0f);
	ImGui::NewLine();                       //空行
	ImGui::SliderFloat("textureBlend", &_blend, 0.0f, 1.0f);                       //新增滑动调整float类型变量调节贴图权重
	ImGui::NewLine();                       //空行
	ImGui::Checkbox("Checkpoint", &_check);                               //新增显示确认与否的词条反馈布尔型变量用作功能展示
	ImGui::NewLine();                       //空行
	//export Mesh
	ImGui::Text("Mesh Export");
	ImGui::Separator();
	ImGui::NewLine();
	_exportMesh = ImGui::IsItemClicked(ImGui::Button("Export Mesh"));
	ImGui::End();                           //ui结束


}

ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//绘制ui
}