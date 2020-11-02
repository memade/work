#if !defined(AFX_SKSTU_DISKLOG_H__9D64D_35ABC402_5D26DC5FFFCFEBB_65A6__HEAD__)
#define AFX_SKSTU_DISKLOG_H__9D64D_35ABC402_5D26DC5FFFCFEBB_65A6__HEAD__

#include "../common/File.hpp"

namespace shared {
	//! 每日每个日志文件最大 64M
	template<typename T>
	class DiskLog final {
	public:
		DiskLog(const char* pFilePathDirectory, sk::int64 MaxSignalFileLen = sizeof(T) * 1024, const char* pFileSuffix = ".sklog") :
			m_LogFileSuffix(pFileSuffix),
			m_MaxSignalFileLen(MaxSignalFileLen),
			m_LogPath(pFilePathDirectory) {
			Open();
		}
		~DiskLog() {
			Close();
		}
	private:
		int Close() {
			m_pMapFile->Close();
			SK_DELETE_PTR(m_pMapFile);
			return 0;
		}
		int Open() {
			sk::Helper::CreateDirectoryA(m_LogPath.c_str());
			size_t FileIndex = 0;
			sk::stringa FileName = sk::time::Time::GetSysdateStr("%04d-%02d-%02d_");
			std::map<std::string, _finddata_t> mapFiles;
			sk::Helper::EnumFile(m_LogPath.c_str(), mapFiles, ("*" + m_LogFileSuffix).c_str());
			if (!mapFiles.empty()) {
				sk::stringa FileObjectLast;
				for (auto it = mapFiles.begin(); it != mapFiles.end(); ++it) {
					FileObjectLast = it->first;
				}
				if (FileObjectLast.find(FileName) != sk::stringa::npos) {
					m_pMapFile = new sk::file::MapFile((m_LogPath + FileObjectLast).c_str(), FileObjectLast.c_str());
					m_pMapFile->Open();
					auto Begin = m_pMapFile->Read();
					auto Total = m_pMapFile->GetDataLength();
					decltype(Total) pos = 0;
					do {
						if (!Begin) {
							break;
						}
						if (Total < sizeof(T)) {
							break;
						}
						if (m_NewestQ.size() >= 100) {
							break;
						}
						auto pNode = (T*)(Begin + pos);
						if (!pNode) {
							break;
						}
						m_NewestQ.push_back(*pNode);
						Total -= sizeof(T);
						pos += sizeof(T);
					} while (1);
					return 1;
				}
			}
			FileName.append(tostringa(FileIndex)).append(m_LogFileSuffix);
			m_pMapFile = new sk::file::MapFile((m_LogPath + FileName).c_str(), FileName.c_str());
			m_pMapFile->Open();
			return 0;
		}
		int TurnThePage() {//! 翻篇
			/*std::lock_guard<std::mutex> lock(m_mutex);*/
			size_t FileIndex = 0;
			sk::stringa FileName = sk::time::Time::GetSysdateStr("%04d-%02d-%02d_");
			std::map<std::string, _finddata_t> mapFiles;
			sk::Helper::EnumFile(m_LogPath.c_str(), mapFiles, ("*" + m_LogFileSuffix).c_str());
			if (!mapFiles.empty()) {
				for (auto it = mapFiles.begin(); it != mapFiles.end(); ++it) {
					if (it->first.find(FileName) == sk::stringa::npos) {
						continue;
					}
					auto findBegin = it->first.find("_");
					auto findEnd = it->first.find(".");
					findBegin++;
					if (findBegin == sk::stringa::npos || findEnd == sk::stringa::npos) {
						continue;
					}
					size_t index = ::strtoll(it->first.substr(findBegin, findEnd - findBegin).c_str(), nullptr, 10);
					if (index > FileIndex) FileIndex = index;
				}
				++FileIndex;
			}

			m_pMapFile->Close();
			SK_DELETE_PTR(m_pMapFile);
			FileName.append(tostringa(FileIndex)).append(m_LogFileSuffix);
			m_pMapFile = new sk::file::MapFile((m_LogPath + FileName).c_str(), FileName.c_str());
			m_pMapFile->Open();

			return 0;
		}
	public:
		std::shared_ptr<sk::container::queue<T>> PopNewest() {
			std::shared_ptr<sk::container::queue<T>> result;
			if (m_NewestQ.empty()) return result;
			std::lock_guard<std::mutex> lock(m_mutex);
			result = std::make_shared<sk::container::queue<T>>();
			m_NewestQ.iterate([&](auto& obj) {result->push(obj); });
			return result;
		}
		int Write(const T& Data) {
			if (!m_pMapFile) return -1;
			std::lock_guard<std::mutex> lock(m_mutex);

			auto prev = sk::time::Time::TimestampToTime(Data.TimeUpdate);
			auto now = sk::time::Time::TimestampToTime(::time(0));
			if (((prev - (prev % 1000000)) != (now - (now % 1000000))) ||
				m_pMapFile->GetDataLength() >= m_MaxSignalFileLen) {//! 跨天和超载，翻页~
				m_NewestQ.clear();
				TurnThePage();
			}

			if (m_NewestQ.size() >= 100) {//! 缓冲最多100条最新
				m_NewestQ.pop_back();
				m_NewestQ.push_front(Data);
			}
			else {
				m_NewestQ.push_front(Data);
			}

			if (!m_pMapFile) return -1;
			m_pMapFile->Write((char*)&Data, (unsigned long)sizeof(Data));
			return 0;
		}
		const T* Read(sk::int64 Offset = 0) const {
			if (!m_pMapFile) return -1;
			std::lock_guard<std::mutex> lock(m_mutex);
			return reinterpret_cast<T*>(m_pMapFile->Read());
		}
	private:
		sk::container::list<T> m_NewestQ;/*取一定数最最新的缓冲 100个怎样*/
		std::mutex m_mutex;
		sk::stringa m_LogPath;
		sk::stringa m_LogFileSuffix;
		sk::int64 m_MaxSignalFileLen;
		sk::file::MapFile* m_pMapFile = nullptr;
	};



}///namespace shared




/// /*新生联创（上海）**/
/// /*2020年02月15日 16:21:37|499**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_DISKLOG_H__9D64D_35ABC402_5D26DC5FFFCFEBB_65A6__TAIL__

