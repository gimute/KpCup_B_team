#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/BoxCollider.h"
//参考：廣田先生のゲームから

struct CellBinary{
	Vector3		vecTexPos[3];	//セルの頂点のポジション
	int			lincCellNo[3];	//セルに隣接しているセル
	Vector3		cellCenterPos;	//セルの真ん中のポジション
};

//セルクラス
struct Cell
{
	enum State {
		State_NotResearch,
		State_Opened,
		State_Closed,
	};
	Vector3		vertexPos[3];	//セルの頂点のポジション
	Cell*		linkCells[3];	//セルに隣接しているセル
	Vector3		cellCenterPos;	//セルの真ん中のポジション
	int			linkMax = 0;	//隣接しているセルの個数
	float		costFromStart;
	float		costToGoal;
	Cell*		parentCell;
	State		state = State_NotResearch;
};

class Game;
class BackGround;

class Navimesh : public IGameObject
{
public: 
	Navimesh();
	~Navimesh();
	bool Start() override { return true; };
	void Update() override;
	void Init(BackGround* floor);
	void Create(const Model& model);
	const std::vector<Cell*>& GetCell() const
	{
		
	}
private:
	/// <summary>
	/// セルの隣接情報を構築する。
	/// </summary>
	/// <param name="startCellNo">開始セルの番号。</param>
	/// <param name="endCellNo">終了セルの番号。</param>
	void BuildLinkCellInfo(int startCellNo, int endCellNo);
	/// <summary>
	/// モデルデータからナビメッシュ構築用の頂点バッファとインデックスバッファを構築する。
	/// </summary>
	/// <param name="model"></param>
	void BuildVertexBufferAndIndexBufferFrom(const Model& model);
	/// <summary>
	/// セルを構築する。
	/// </summary>
	void BuildCells();
private:
	//typedefは既存の型名に新しい名前を付ける。
	typedef std::vector<Vector3>					VertexBuffer;	//頂点バッファ。
	typedef std::vector<unsigned int>				IndexBuffer;	//インデックスバッファ。
	//unique_ptrはコピー不可能なクラス。
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//頂点バッファの配列。
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//インデックスバッファの配列。

	BoxCollider			m_collider;						//セル用のカプセルコライダー。
	const float			COLLIDER_EXPANSION = 2.45f;		//コライダーの拡大倍率。元々は2.45f。
	std::vector<Cell*>	m_cells;						//セルの配列。

	//デバッグ関係。
	ModelRender* m_debugModel = nullptr;			//デバッグモデル。
	std::vector<ModelRender*>	m_debugModelList;				//デバッグモデルの配列。
	bool							m_isDebug = false;				//デバッグモードのオンオフ。
	bool							m_isChangeActiveModel = false;	//デバッグモードのオンオフ。
	bool							m_isCreateModel = false;		//デバッグモデルを作ったかどうか。
};

