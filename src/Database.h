#pragma once

#include "Util.h"

namespace DB
{
	std::set<std::string> aggressive_class{ "Ro", "HhPJ", "HhBj", "HhPo", "SJ" };
	std::set<std::string> aggressive_mod{ "BG", "BB" };

	json BuildJson(pugi::xml_document& xml_doc, const fs::path& mod_path, const fs::path& cls_path)
	{
		auto j_obj = json::object();
		std::vector<std::string> split_id;

		if (auto scene = xml_doc.child("scene")) {
			if (auto id = scene.attribute("id")) {
				auto id_val = id.value();
				j_obj["sceneid"] = id_val;
				split_id = Util::StringSplit(id_val, '|');
			}

			if (auto actors = scene.attribute("actors"))
				j_obj["NumActors"] = actors.as_int();

			if (auto info = scene.child("info"))
				if (auto name = info.attribute("name"))
					j_obj["name"] = name.value();

			j_obj["OAanimids"] = json::array();

			int is_transitory = 0;
			int is_hub = 1;

			if (auto anim = scene.child("anim"))
				if (auto transitory = anim.attribute("t")) {
					std::string trans_str{ transitory.value() };
					if (trans_str == "T"s) {
						is_transitory = 1;
						is_hub = 0;
					}
				}

			j_obj["istransitory"] = is_transitory;

			auto speed = scene.child("speed");
			if (speed && !is_transitory) {
				if (auto max = speed.attribute("max"))
						is_hub = 0;

				if (!is_hub) {
					if (auto actor = speed.attribute("a"))
						j_obj["mainActor"] = actor.as_int();

					if (auto min = speed.attribute("min"))
						j_obj["minspeed"] = min.as_int();

					if (auto max = speed.attribute("max"))
						j_obj["maxspeed"] = max.as_int();

					j_obj["hasIdleSpeed"] = 0;

					for (auto& sp : speed.children("sp")) {
						if (auto mtx = sp.attribute("mtx")) {
							std::string mtx_str{ mtx.value() };
							if (mtx_str == "^idle"s)
								j_obj["hasIdleSpeed"] = 1;
						}

						if (auto anim = sp.child("anim"))
							if (auto id = anim.attribute("id"))
								j_obj["OAanimids"].push_back(id.value());
					}
				}
			}

			j_obj["ishub"] = is_hub;

			if (j_obj["OAanimids"].empty()) {
				auto anim_children = scene.children("anim");
				auto anim_count = std::distance(anim_children.begin(), anim_children.end());
				bool anim_first = true;

				for (auto& anim : anim_children) {
					if (anim_first && (anim_count > 1)) {
						anim_first = false;
						continue;
					}

					if (auto id = anim.attribute("id"))
						j_obj["OAanimids"].push_back(id.value());
				}
			}

			// what the hell?
			if (auto animations = scene.child("animation")) {
				auto role_children = animations.children("animrole");
				auto role_count = std::distance(role_children.begin(), role_children.end());
				bool role_first = true;

				for (auto& role : role_children) {
					if (role_first && (role_count > 1)) {
						role_first = false;
						continue;
					}

					if (auto anim = role.child("animplan").child("anim"))
						if (auto id = anim.attribute("id"))
							j_obj["OAanimids"].push_back(id.value());
				}
			}

		}

		auto anim_class = cls_path.filename().string();
		auto source_mod = mod_path.filename().string();

		if (anim_class == "PO")
			anim_class = "Po";

		int is_aggressive = 0;
		if (aggressive_class.find(anim_class) != aggressive_class.end())
			is_aggressive = 1;
		else if (aggressive_mod.find(source_mod) != aggressive_mod.end())
			is_aggressive = 1;
		else if (anim_class.find("Ag") != std::string::npos)
			is_aggressive = 1;

		j_obj["aggressive"] = is_aggressive;
		j_obj["sourcemodule"] = source_mod;
		j_obj["animclass"] = anim_class;
		j_obj["positiondata"] = split_id[1];

		return j_obj;
	}

	void Build()
	{
		auto timer_start = std::chrono::high_resolution_clock::now();

		const fs::path root_path("Data/Meshes/0SA/mod/0Sex/scene");

		if (!fs::exists(root_path)) {
			logger::info("scene folder not found");
			return;
		}

		auto j_root = json::array();
		
		for (auto const& mod : fs::directory_iterator(root_path)) {
			auto& mod_path = mod.path();

			for (auto& pos : fs::directory_iterator(mod_path)) {
				auto& pos_path = pos.path();

				auto posName = pos_path.filename().string();
				if (posName == "_TOG")
					continue;

				for (auto& cls : fs::directory_iterator(pos_path)) {
					auto& cls_path = cls.path();

					for (auto& file : fs::directory_iterator(cls_path)) {
						if (!fs::exists(file) || !fs::is_regular_file(file))
							continue;

						auto& file_path = file.path();

						auto fileExt = file_path.extension().string();
						if (fileExt != ".xml")
							continue;

						auto file_path_str = file_path.string();

						pugi::xml_document xml_doc;
						pugi::xml_parse_result xml_result = xml_doc.load_file(file_path_str.c_str());

						if (!xml_result) {
							logger::error("load failed: {} [{}]", file_path_str, xml_result.description());
							continue;
						}

						auto j_build = BuildJson(xml_doc, mod_path, cls_path);
						j_root.push_back(j_build);
					}
				}
			}
		}

		auto db_path = Util::GetDatabasePath()->string();
		std::ofstream db_file(db_path);
		db_file << std::setw(2) << j_root << std::endl;

		auto timer_finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> timer_elapsed = timer_finish - timer_start;
		logger::info("timer: {}", timer_elapsed.count());
	}
}