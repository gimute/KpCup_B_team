#pragma once

namespace nsK2EngineLow {
	class RenderingEngine : public Noncopyable
	{
	public:

		RenderingEngine();
		~RenderingEngine();

		//�`��I�u�W�F�N�g��ǉ�
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		//3D���f���̕`��
		void Render3DModel(RenderContext& rc);

		//2D�`��
		void Render2D(RenderContext& rc);

		void Execute(RenderContext& rc);

	private:
		std::vector< IRenderer* > m_renderObjects;                      // �`��I�u�W�F�N�g�̃��X�g�B
	};
}


