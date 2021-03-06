// Viterbi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <map>
/**
* 求解HMM模型
* @param obs 观测序列
* @param states 隐状态
* @param start_p 初始概率（隐状态）
* @param trans_p 转移概率（隐状态）
* @param emit_p 发射概率 （隐状态表现为显状态的概率）
* @return 最可能的序列
*/
std::vector<std::string> compute(std::vector<std::string> obs, std::vector<std::string> states,
	std::map<std::string, double> start_p,
	std::map<std::string, std::map<std::string, double>> trans_p,
	std::map<std::string, std::map<std::string, double>> emit_p)
{
	std::map<std::string, std::vector<std::string>> path;
	std::map<std::string, double> curr_pro;

	for (auto s : states)
		//先假设所有的值是已经存在的，不需要去搜索
		curr_pro[s] = start_p[s] * emit_p[s][obs[0]];

	for (auto i = 1; i < (int)obs.size(); i++)
	{
		std::map<std::string, double> last_pro(curr_pro);
		curr_pro.clear();
		for (auto curr_state : states)
		{
			struct X
			{
				double value = 0.0;
				std::string state;
			};
			std::vector<X> seq;
			for (auto last_state : states)
			{
				X result;
				result.value = last_pro[last_state] * trans_p[last_state][curr_state] * emit_p[curr_state][obs[i]];
				result.state = last_state;
				seq.push_back(result);
			}
			auto max_X = std::max_element(seq.begin(), seq.end(), [](X item1,X item2) {
				if (item1.value < item2.value) return true; return false;
			});

			curr_pro[curr_state] = max_X->value;
			path[curr_state].push_back(max_X->state);
		}
	}

	auto max_pro = -1.0;
	std::vector<std::string> max_path;
	for (auto s : states)
	{
		path[s].push_back(s);
		if (curr_pro[s] > max_pro)
		{
			max_path = path[s];
			max_pro = curr_pro[s];
		}
		std::cout << curr_pro[s] << std::endl;
	}
	return max_path;
}

int main()
{
	std::vector<std::string> obs;
	obs.push_back("normal");
	obs.push_back("cold");
	obs.push_back("dizzy");
	std::vector<std::string> states;
	states.push_back("Healthy");
	states.push_back("Fever");

	std::map<std::string, double> start_p;
	start_p["Healthy"] = 0.6;
	start_p["Fever"] = 0.4;
	std::map<std::string, std::map<std::string, double>> trans_p;
	std::map<std::string, double> trans;
	trans["Healthy"] = 0.7;
	trans["Fever"] = 0.3;
	trans_p["Healthy"] = trans;
	trans["Healthy"] = 0.4;
	trans["Fever"] = 0.6;
	trans_p["Fever"] = trans;
	std::map<std::string, std::map<std::string, double>> emit_p;
	std::map<std::string, double> emit;
	emit["normal"] = 0.5;
	emit["cold"] = 0.4;
	emit["dizzy"] = 0.1;
	emit_p["Healthy"] = emit;
	emit["normal"] = 0.1;
	emit["cold"] = 0.3;
	emit["dizzy"] = 0.6;
	emit_p["Fever"] = emit;

	//这里仅是通过调试的方式查看结果
	auto result = compute(obs, states, start_p, trans_p, emit_p);
	getchar();
	return 0;
}

