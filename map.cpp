#include "map.h"
#include "node.h"
#include "scanner.h"
#include "token.h"

namespace YAML
{
	Map::Map()
	{
	}

	Map::~Map()
	{
		for(node_map::const_iterator it=m_data.begin();it!=m_data.end();++it) {
			delete it->first;
			delete it->second;
		}
	}

	void Map::Parse(Scanner *pScanner)
	{
		// grab start token
		Token *pToken = pScanner->GetNextToken();

		switch(pToken->type) {
			case TT_BLOCK_MAP_START: ParseBlock(pScanner); break;
			case TT_FLOW_MAP_START: ParseFlow(pScanner); break;
		}

		delete pToken;
	}

	void Map::ParseBlock(Scanner *pScanner)
	{
		while(1) {
			Token *pToken = pScanner->PeekNextToken();
			if(!pToken)
				break;  // TODO: throw?

			if(pToken->type != TT_KEY && pToken->type != TT_BLOCK_END)
				break;  // TODO: throw?

			pScanner->PopNextToken();
			if(pToken->type == TT_BLOCK_END)
				break;

			Node *pKey = new Node;
			Node *pValue = new Node;
			m_data[pKey] = pValue;

			// grab key
			pKey->Parse(pScanner);

			// now grab value (optional)
			if(pScanner->PeekNextToken() && pScanner->PeekNextToken()->type == TT_VALUE) {
				pScanner->PopNextToken();
				pValue->Parse(pScanner);
			}
		}
	}

	void Map::ParseFlow(Scanner *pScanner)
	{
		while(1) {
			Token *pToken = pScanner->PeekNextToken();
			if(!pToken)
				break;  // TODO: throw?

			// first check for end
			if(pToken->type == TT_FLOW_MAP_END) {
				pScanner->EatNextToken();
				break;
			}

			// now it better be a key
			if(pToken->type != TT_KEY)
				break;  // TODO: throw?

			pScanner->PopNextToken();

			Node *pKey = new Node;
			Node *pValue = new Node;
			m_data[pKey] = pValue;

			// grab key
			pKey->Parse(pScanner);

			// now grab value (optional)
			if(pScanner->PeekNextToken() && pScanner->PeekNextToken()->type == TT_VALUE) {
				pScanner->PopNextToken();
				pValue->Parse(pScanner);
			}

			// now eat the separator (or could be a map end, which we ignore - but if it's neither, then it's a bad node)
			pToken = pScanner->PeekNextToken();
			if(pToken->type == TT_FLOW_ENTRY)
				pScanner->EatNextToken();
			else if(pToken->type != TT_FLOW_MAP_END)
				break;  // TODO: throw?
		}
	}

	void Map::Write(std::ostream& out, int indent)
	{
		for(int i=0;i<indent;i++)
			out << "  ";
		out << "{map}\n";

		for(node_map::const_iterator it=m_data.begin();it!=m_data.end();++it) {
			for(int i=0;i<indent + 1;i++)
				out << "  ";
			out << "{key}\n";
			it->first->Write(out, indent + 2);

			for(int i=0;i<indent + 1;i++)
				out << "  ";
			out << "{value}\n";
			it->second->Write(out, indent + 2);
		}
	}
}