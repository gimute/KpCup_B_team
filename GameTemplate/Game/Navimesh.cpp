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
	/// 備考
		//CallBackクラスのコンストラクタ
		//CallBack()
		//初期化リスト
		//{}の前に初期化を行う
		//namespace　btCollisionWorld
		//構造体　ClosestConvexResultCallback
		//ClosestConvexResultCallbackはコンストラクタに引数があるので
		//初期化しなくてはならない
		//{}
	/// </summary>

	//障害物があるかないか判定
	bool isHit = false;
	//衝突したら勝手に呼んでくれる
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//当たった
		isHit = true;
		return 0;
	}
};
/*!
	*@brief	ストップウォッチクラス。
	*@details
	* C#のStopwatchのように使える。</br>
	*@code
	サンプルコード
	Stopwatch sw;
	sw.Start();		//計測開始。
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//計測終了
	printf("経過時間 = %lf(単位：秒)\n", sw.GetElapsed());
	printf("経過時間 = %lf(単位：ミリ)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
class CStopwatch {
public:
	/*!
		*@brief	コンストラクタ。
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
		*@brief	デストラクタ。
		*/
	~CStopwatch()
	{
	}
	/*!
		*@brief	計測開始。
		*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
		*@brief	計測終了
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
		*@brief	経過時間を取得(単位:秒)
		*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
		*@brief	経過時間を取得(単位:ミリ秒)
		*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
		*@brief	経過時間を取得(単位:マイクロ秒)
		*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//経過時間(単位：秒)
	double elapsedMill;		//経過時間(単位：ミリ秒)
	double elapsedMicro;	//経過時間(単位：マイクロ秒)

};

void Navimesh::BuildLinkCellInfo(int startCellNo, int endCellNo)
{
	//登録されたセルの数だけ回す
	for (int cellNo = startCellNo; cellNo < endCellNo && cellNo < m_cells.size(); cellNo++)
	{
		auto all = m_cells[cellNo];

		//同じセルと比較しないようにするためのループ
		for (auto &currentCell : m_cells)
		{
			//同じセルなら除外する
			if (all != currentCell)
			{
				//座標が一致した頂点をカウント
				int countSameVertecx = 0;
				//allの頂点要素を進める
				for (int i = 0; i < 3; i++)
				{
					Vector3 allPos;
					allPos = all->vertexPos[i];

					//currentCellの頂点の要素を進める
					for (int j = 0; j < 3; j++)
					{
						//allPosとcurrentPosの距離
						Vector3 diff;
						diff = allPos - currentCell->vertexPos[j];
						//もし頂点同士がほぼ同じ距離にいたら
						if (diff.Length() <= 0.01f)
						{
							//一致した頂点数をカウント
							countSameVertecx++;
						}
					}
				}
				//２頂点が重複していたら、隣接しているとする
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
	//コールバック関数
	model.FindMesh([&](const auto& mesh) {
		ID3D12GraphicsCommandList* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//頂点バッファを作成
		{
			//サブリソースにアクセス
			ID3D12Resource* resource = nullptr;
			//ポインタデータ
			UINT8* pData;
			//書き込み専用の場合、読み取り範囲は0に設定
			D3D12_RANGE readRange = { 0, 0 };
			//リソースをmapする
			HRESULT hr = resource->Map(0,&readRange, 
				reinterpret_cast<void**>(&pData));
			//エラーを確認
			if (FAILED(hr)) {
				return;
			}
			//データをコピー
			memcpy(pData, mesh->vertexBufferMicrosoftWRL.Get()
				, sizeof(mesh->vertexBufferMicrosoftWRL.Get()));
			//バッファーリソースに記述するための構造体を宣言
			D3D12_RESOURCE_DESC resourceDesc;
			//バッファリソースのプロパティを取得
			resourceDesc = mesh->vertexBufferMicrosoftWRL->GetDesc();
			int vertexCount = resourceDesc.Width / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(pData);
			VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
			Vector3 pos;
			for (int i = 0; i < vertexCount; i++)
			{
				pos = *reinterpret_cast<Vector3*>(pData);
				//バイアスをかける
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//次の頂点へ
				pData += mesh->vertexStride;
			}
			//リソースのmapを解除
			resource->Unmap(0, nullptr);
			//配列に登録
			m_vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//インデックスバッファ
		{
			//サブリソースにアクセス
			ID3D12Resource* resource = nullptr;
			//ポインタデータ
			UINT8* pData;
			//書き込み専用の場合、読み取り範囲は0に設定
			D3D12_RANGE readRange = { 0, 0 };
			//リソースをmapする
			HRESULT hr = resource->Map(0, &readRange,
				reinterpret_cast<void**>(&pData));
			//エラーを確認
			if (FAILED(hr)) {
				return;
			}
			//バッファーリソースに記述するための構造体を宣言
			D3D12_RESOURCE_DESC resourceDesc;
			//バッファリソースのプロパティを取得
			resourceDesc = mesh->vertexBufferMicrosoftWRL->GetDesc();
			//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = resourceDesc.Width / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(pData);
			for (int i = 0; i < indexCount; i++)
			{
				indexBuffer->push_back(pIndex[i]);
			}
			//リソースのmapを解除
			resource->Unmap(0, nullptr);
			//配列に登録
			m_indexBufferArray.push_back(std::move(indexBuffer));
		}
	}
	);
}

void Navimesh::BuildCells()
{
	//セルに頂点3つと重心を設定し、セルに登録
	//m_vertexBufferArrayとm_indexBufferArrayの要素数は同じ
	for (int i = 0; i < m_indexBufferArray.size(); i++)
	{
		//インデックスバッファの配列
		//頂点の場所
		auto& indexBuffer = m_indexBufferArray[i];
		//頂点バッファの配列
		//一つの頂点バッファにつき1パーツ
		auto& vertexBuffer = m_vertexBufferArray[i];

		//ポリゴンの数
		//ポリゴンの頂点は3つで形成されるので3で割っている
		int numPoly = indexBuffer->size() / 3;
		//ポリゴンの数だけ回す
		for (int polyNo = 0; polyNo < numPoly; polyNo++){
			//セルの生成
			Cell* cell = new Cell;

			//頂点の格納
			{
				//ポリゴン１個に対する３か所の頂点の場所を割り出す
				int vertNo_0 = indexBuffer->at(polyNo * 3);
				int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
				int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
				//セルに頂点の場所を格納
				cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
				cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
				cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
			}

			//centerPosの登録
			//三角形の重心
			//合計なので初期化
			cell->cellCenterPos = Vector3::Zero;
			cell->cellCenterPos = cell->vertexPos[0] + cell->vertexPos[1] + cell->vertexPos[2];
			cell->cellCenterPos /= 3;

			//カプセルコライダーの半径を
			//三角形の一番長い重心頂点間の長さに拡大

			//重心から頂点までの長さを求める
			//これがカプセルコライダーの半径になる
			Vector3 radius[3];
			for (int i = 0; i < 3; i++)
			{
				radius[i] = cell->cellCenterPos - cell->vertexPos[i];
			}
			//1番長い距離を比較で求める
			Vector3 maxLength;
			maxLength = radius[0];
			for (int i = 0; i < 3; i++)
			{
				if (maxLength.Length() < radius[i].Length())
				{
					maxLength = radius[i];
				}
			}
			//カプセルコライダーの半径を拡大
			Vector3 halfSize;
			halfSize.x = maxLength.Length() * COLLIDER_EXPANSION;
			halfSize.z = maxLength.Length() * COLLIDER_EXPANSION;
			halfSize.y = 30.0f;
			m_collider.Create(halfSize);

			//コリジョンの移動の始点と終点の設定
			//btTransformは座標、回転の情報を持っている
			//情報の初期化を行わなければならない
			btTransform start, end;
			{
				//回転の設定
				start.setIdentity();
				end.setIdentity();
				//座標の設定
				//原点の設定
				start.setOrigin(btVector3(cell->cellCenterPos.x
					, cell->cellCenterPos.y + 20.0f, cell->cellCenterPos.z));
				end.setOrigin(btVector3(cell->cellCenterPos.x
					, cell->cellCenterPos.y + 50.0f, cell->cellCenterPos.z));
			}

			 NMCallBack callback;
			 //startからendまでコリジョンを移動させて当たり判定を取る
			 PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

			 if (callback.isHit == false)
			 {
				 //セルに登録
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
	//一つのスレッドで調べるセルの数を計算
	CStopwatch sw;
	sw.Start();
#endif
	int numCellOneThread = m_cells.size() / 3;
#if 1 //これを0にするとシングルスレッドでの隣接セルの構築になります。
	//4スレッドに分担して隣接セル情報を構築する。
	//並列。
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
	//直列
	buildLinkCellThread00.join();
	buildLinkCellThread01.join();
	buildLinkCellThread02.join();
	buildLinkCellThread03.join();
#endif
#ifdef _DEBUG
	//時間計測
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
		//ファイルオープンに失敗
		MessageBox(nullptr, L"エラー", L"stage.hnvが開けませんでした。", MB_OK);
	}
	int cellNum = m_cells.size();
	//バイナリデータをファイルに書き込む
	fwrite(&cellNum, sizeof(int), 1, fp);
	//セルのデータを構造体に保存する
	for (auto& cell : m_cells)
	{
		CellBinary cellBinary;
		//頂点
		for (int i = 0; i < 3; i++)
		{
			cellBinary.vecTexPos[i] = cell->vertexPos[i];
		}
		//隣接セル
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