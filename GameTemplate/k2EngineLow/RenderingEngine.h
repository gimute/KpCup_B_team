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

		//���O2D�`��
		void PreRender2D(RenderContext& rc);

		//�V���h�E�}�b�v�`�揈��
		void RenderShadowMap(RenderContext& rc);

		//���s
		void Execute(RenderContext& rc);


		RenderTarget* GetShadowMapRenderTarget()
		{
			return &m_shadowMap;
		}

	private:
		std::vector< IRenderer* > m_renderObjects;	// �`��I�u�W�F�N�g�̃��X�g�B

		RenderTarget m_shadowMap;
	};
}


