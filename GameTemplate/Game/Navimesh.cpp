#include "stdafx.h"
#include "Navimesh.h"
#include <thread>

Navimesh::Navimesh()
{

}

Navimesh::~Navimesh()
{

}

void Navimesh::Update()
{

}

struct NMCallBack : public btCollisionWorld::ConvexResultCallback
{
	/// <summary>
	/// ���l
		//CallBack�N���X�̃R���X�g���N�^
		//CallBack()
		//���������X�g
		//{}�̑O�ɏ��������s��
		//namespace�@btCollisionWorld
		//�\���́@ClosestConvexResultCallback
		//ClosestConvexResultCallback�̓R���X�g���N�^�Ɉ���������̂�
		//���������Ȃ��Ă͂Ȃ�Ȃ�
		//{}
	/// </summary>

	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//��������
		isHit = true;
		return 0;
	}
};
/*!
	*@brief	�X�g�b�v�E�H�b�`�N���X�B
	*@details
	* C#��Stopwatch�̂悤�Ɏg����B</br>
	*@code
	�T���v���R�[�h
	Stopwatch sw;
	sw.Start();		//�v���J�n�B
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//�v���I��
	printf("�o�ߎ��� = %lf(�P�ʁF�b)\n", sw.GetElapsed());
	printf("�o�ߎ��� = %lf(�P�ʁF�~��)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
class CStopwatch {
public:
	/*!
		*@brief	�R���X�g���N�^�B
		*/
	CStopwatch()
	{
		freq = 0;
		end = 0;
		begin = 0;
		elapsed = 0.0;
		elapsedMill = 0.0;
		elapsedMicro = 0.0;
	}
	/*!
		*@brief	�f�X�g���N�^�B
		*/
	~CStopwatch()
	{
	}
	/*!
		*@brief	�v���J�n�B
		*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
		*@brief	�v���I��
		*/
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�b)
		*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�~���b)
		*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�}�C�N���b)
		*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)

};

void Navimesh::BuildLinkCellInfo(int startCellNo, int endCellNo)
{
	//�o�^���ꂽ�Z���̐�������
	for (int cellNo = startCellNo; cellNo < endCellNo && cellNo < m_cells.size(); cellNo++)
	{
		auto all = m_cells[cellNo];

		//�����Z���Ɣ�r���Ȃ��悤�ɂ��邽�߂̃��[�v
		for (auto &currentCell : m_cells)
		{
			//�����Z���Ȃ珜�O����
			if (all != currentCell)
			{
				//���W����v�������_���J�E���g
				int countSameVertecx = 0;
				//all�̒��_�v�f��i�߂�
				for (int i = 0; i < 3; i++)
				{
					Vector3 allPos;
					allPos = all->vertexPos[i];

					//currentCell�̒��_�̗v�f��i�߂�
					for (int j = 0; j < 3; j++)
					{
						//allPos��currentPos�̋���
						Vector3 diff;
						diff = allPos - currentCell->vertexPos[j];
						//�������_���m���قړ��������ɂ�����
						if (diff.Length() <= 0.01f)
						{
							//��v�������_�����J�E���g
							countSameVertecx++;
						}
					}
				}
				//�Q���_���d�����Ă�����A�אڂ��Ă���Ƃ���
				if (countSameVertecx == 2)
				{
					all->linkCells[all->linkMax] = currentCell;
					all->linkMax++;

				}

			}

		}

	}
}

void Navimesh::BuildVertexBufferAndIndexBufferFrom(const Model& model)
{
	Matrix mBias = model.GetWorldMatrix();
	//�R�[���o�b�N�֐�
	model.FindMesh([&](const auto& mesh) {
		ID3D12GraphicsCommandList* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//���_�o�b�t�@���쐬
		{
			//�T�u���\�[�X�ɃA�N�Z�X
			ID3D12Resource* resource = nullptr;
			//�|�C���^�f�[�^
			UINT8* pData;
			//�������ݐ�p�̏ꍇ�A�ǂݎ��͈͂�0�ɐݒ�
			D3D12_RANGE readRange = { 0, 0 };
			//���\�[�X��map����
			HRESULT hr = resource->Map(0,&readRange, 
				reinterpret_cast<void**>(&pData));
			//�G���[���m�F
			if (FAILED(hr)) {
				return;
			}
			//�f�[�^���R�s�[
			memcpy(pData, mesh->vertexBufferMicrosoftWRL.Get()
				, sizeof(mesh->vertexBufferMicrosoftWRL.Get()));
			//�o�b�t�@�[���\�[�X�ɋL�q���邽�߂̍\���̂�錾
			D3D12_RESOURCE_DESC resourceDesc;
			//�o�b�t�@���\�[�X�̃v���p�e�B���擾
			resourceDesc = mesh->vertexBufferMicrosoftWRL->GetDesc();
			int vertexCount = resourceDesc.Width / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(pData);
			VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
			Vector3 pos;
			for (int i = 0; i < vertexCount; i++)
			{
				pos = *reinterpret_cast<Vector3*>(pData);
				//�o�C�A�X��������
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//���̒��_��
				pData += mesh->vertexStride;
			}
			//���\�[�X��map������
			resource->Unmap(0, nullptr);
			//�z��ɓo�^
			m_vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//�C���f�b�N�X�o�b�t�@
		{
			//�T�u���\�[�X�ɃA�N�Z�X
			ID3D12Resource* resource = nullptr;
			//�|�C���^�f�[�^
			UINT8* pData;
			//�������ݐ�p�̏ꍇ�A�ǂݎ��͈͂�0�ɐݒ�
			D3D12_RANGE readRange = { 0, 0 };
			//���\�[�X��map����
			HRESULT hr = resource->Map(0, &readRange,
				reinterpret_cast<void**>(&pData));
			//�G���[���m�F
			if (FAILED(hr)) {
				return;
			}
			//�o�b�t�@�[���\�[�X�ɋL�q���邽�߂̍\���̂�錾
			D3D12_RESOURCE_DESC resourceDesc;
			//�o�b�t�@���\�[�X�̃v���p�e�B���擾
			resourceDesc = mesh->vertexBufferMicrosoftWRL->GetDesc();
			//@todo cmo�t�@�C���̓C���f�b�N�X�o�b�t�@�̃T�C�Y��2byte�Œ�B
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = resourceDesc.Width / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(pData);
			for (int i = 0; i < indexCount; i++)
			{
				indexBuffer->push_back(pIndex[i]);
			}
			//���\�[�X��map������
			resource->Unmap(0, nullptr);
			//�z��ɓo�^
			m_indexBufferArray.push_back(std::move(indexBuffer));
		}
	}
	);
}

void Navimesh::BuildCells()
{
	//�Z���ɒ��_3�Əd�S��ݒ肵�A�Z���ɓo�^
	//m_vertexBufferArray��m_indexBufferArray�̗v�f���͓���
	for (int i = 0; i < m_indexBufferArray.size(); i++)
	{
		//�C���f�b�N�X�o�b�t�@�̔z��
		//���_�̏ꏊ
		auto& indexBuffer = m_indexBufferArray[i];
		//���_�o�b�t�@�̔z��
		//��̒��_�o�b�t�@�ɂ�1�p�[�c
		auto& vertexBuffer = m_vertexBufferArray[i];

		//�|���S���̐�
		//�|���S���̒��_��3�Ō`�������̂�3�Ŋ����Ă���
		int numPoly = indexBuffer->size() / 3;
		//�|���S���̐�������
		for (int polyNo = 0; polyNo < numPoly; polyNo++){
			//�Z���̐���
			Cell* cell = new Cell;

			//���_�̊i�[
			{
				//�|���S���P�ɑ΂���R�����̒��_�̏ꏊ������o��
				int vertNo_0 = indexBuffer->at(polyNo * 3);
				int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
				int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
				//�Z���ɒ��_�̏ꏊ���i�[
				cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
				cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
				cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
			}

			//centerPos�̓o�^
			//�O�p�`�̏d�S
			//���v�Ȃ̂ŏ�����
			cell->cellCenterPos = Vector3::Zero;
			cell->cellCenterPos = cell->vertexPos[0] + cell->vertexPos[1] + cell->vertexPos[2];
			cell->cellCenterPos /= 3;

			//�J�v�Z���R���C�_�[�̔��a��
			//�O�p�`�̈�Ԓ����d�S���_�Ԃ̒����Ɋg��

			//�d�S���璸�_�܂ł̒��������߂�
			//���ꂪ�J�v�Z���R���C�_�[�̔��a�ɂȂ�
			Vector3 radius[3];
			for (int i = 0; i < 3; i++)
			{
				radius[i] = cell->cellCenterPos - cell->vertexPos[i];
			}
			//1�Ԓ����������r�ŋ��߂�
			Vector3 maxLength;
			maxLength = radius[0];
			for (int i = 0; i < 3; i++)
			{
				if (maxLength.Length() < radius[i].Length())
				{
					maxLength = radius[i];
				}
			}
			//�J�v�Z���R���C�_�[�̔��a���g��
			Vector3 halfSize;
			halfSize.x = maxLength.Length() * COLLIDER_EXPANSION;
			halfSize.z = maxLength.Length() * COLLIDER_EXPANSION;
			halfSize.y = 30.0f;
			m_collider.Create(halfSize);

			//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�
			//btTransform�͍��W�A��]�̏��������Ă���
			//���̏��������s��Ȃ���΂Ȃ�Ȃ�
			btTransform start, end;
			{
				//��]�̐ݒ�
				start.setIdentity();
				end.setIdentity();
				//���W�̐ݒ�
				//���_�̐ݒ�
				start.setOrigin(btVector3(cell->cellCenterPos.x
					, cell->cellCenterPos.y + 20.0f, cell->cellCenterPos.z));
				end.setOrigin(btVector3(cell->cellCenterPos.x
					, cell->cellCenterPos.y + 50.0f, cell->cellCenterPos.z));
			}

			 NMCallBack callback;
			 //start����end�܂ŃR���W�������ړ������ē����蔻������
			 PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

			 if (callback.isHit == false)
			 {
				 //�Z���ɓo�^
				 m_cells.push_back(cell);
			 }
		}
	}
	for (auto &all : m_cells)
	{
		for (int i = 0; i < 3; i++)
		{
			all->linkCells[i] = nullptr;
			all->costFromStart = 0.0f;
			all->costToGoal = 0.0f;
			all->parentCell = nullptr;
		}
	}
}

void Navimesh::BuildLinkCellInfo()
{
#ifdef _DEBUG
	//��̃X���b�h�Œ��ׂ�Z���̐����v�Z
	CStopwatch sw;
	sw.Start();
#endif
	int numCellOneThread = m_cells.size() / 3;
#if 1 //�����0�ɂ���ƃV���O���X���b�h�ł̗אڃZ���̍\�z�ɂȂ�܂��B
	//4�X���b�h�ɕ��S���ėאڃZ�������\�z����B
	//����B
	std::thread buildLinkCellThread00([&] {
		BuildLinkCellInfo(0, numCellOneThread);
	});

	std::thread buildLinkCellThread01([&] {
		BuildLinkCellInfo(numCellOneThread, numCellOneThread * 2);
	});

	std::thread buildLinkCellThread02([&] {
		BuildLinkCellInfo(numCellOneThread * 2, numCellOneThread * 3);
	});

	std::thread buildLinkCellThread03([&] {
		BuildLinkCellInfo(numCellOneThread * 3, numCellOneThread * 4);
	});
#else
	//����
	buildLinkCellThread00.join();
	buildLinkCellThread01.join();
	buildLinkCellThread02.join();
	buildLinkCellThread03.join();
#endif
#ifdef _DEBUG
	//���Ԍv��
	sw.Stop();
	char text[256];
	sprintf(text, "build time = %f\n", sw.GetElapsed());
	OutputDebugString(text);
#endif // _DEBUG
}

void Navimesh::Save()
{
	FILE* fp = fopen("stage.hnv", "wb");
	if (fp == nullptr)
	{
		//�t�@�C���I�[�v���Ɏ��s
		MessageBox(nullptr, L"�G���[", L"stage.hnv���J���܂���ł����B", MB_OK);
	}
	int cellNum = m_cells.size();
	//�o�C�i���f�[�^���t�@�C���ɏ�������
	fwrite(&cellNum, sizeof(int), 1, fp);
	//�Z���̃f�[�^���\���̂ɕۑ�����
	for (auto& cell : m_cells)
	{
		CellBinary cellBinary;
		//���_
		for (int i = 0; i < 3; i++)
		{
			cellBinary.vecTexPos[i] = cell->vertexPos[i];
		}
		//�אڃZ��
		for (int i = 0; i < 3; i++)
		{
			int count = 0;
			if (cell->linkCells[i] == nullptr)
			{
				cellBinary.lincCellNo[i] = -1;
			}
			else
			{

			}
		}
	}
}