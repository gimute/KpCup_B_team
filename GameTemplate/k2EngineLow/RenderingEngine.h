#pragma once

namespace nsK2EngineLow {
	class RenderingEngine : public Noncopyable
	{
	public:

		RenderingEngine();
		~RenderingEngine();

		void Init();

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
		RenderTarget m_luminanceRenderTarget;	//�P�x���o�p�̃����_�����O
		Sprite m_luminanceSprite;				//�P�x���o�p�̃X�v���C�g
		RenderTarget m_mainRenderTarget;
		GaussianBlur m_gaussianBulur;

		std::vector< IRenderer* > m_renderObjects;                      // �`��I�u�W�F�N�g�̃��X�g�B
	};
}


