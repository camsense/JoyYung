#ifndef __READ_PARSE_PACKAGE_H__
#define __READ_PARSE_PACKAGE_H__

#include <atomic>
#include <cstdint>
#include <mutex>


#include "rOc_serial.hpp"

// ver. 2021-1-19
#define PACKAGESIZE 128

///////////2023-06-02
#define MIN_BLOCK_SPEED             80
#define BLOCK_SPEED_CONTINUE_MS     3500

#ifndef BYTE
typedef unsigned char BYTE;
#endif

struct rangedata {
  bool flag;          // 0->valid, 1->invalid
  float angle;        // degree
  unsigned int dist;  // millimeter
  unsigned int speed;
  uint16_t syn_quality;
  rangedata() : flag(false), angle(0.), dist(0), speed(0), syn_quality(0) {}
};

enum lidarErrorProcessCode {
  LIDAR_SUCCESS = 0,
  //     ʹ  ڴ     
  ERR_SERIAL_INVALID_HANDLE         = -1001,  //   ھ  Ϊ  
  ERR_SERIAL_SETCOMMTIMEOUTS_FAILED = -1002,  //   ô  ڶ ȡ  ʱ    ʧ  
  ERR_SERIAL_READFILE_FAILED        = -1003,  //  ȡ    ʧ  
  ERR_SERIAL_READFILE_ZERO          = -1004,  //   ڶ ȡ     ֽ 
  ERR_FIND_HEAD_TIMEOUT             = -1005,  //   Ұ ͷʧ  
  ERR_CHECKDATA_INVALID             = -1006,  //У   ʧ  
  ERR_GETPACKAGE_FAILED             = -1007,  //  ȡ  ʧ  
  ERR_DATABYTELENGTH_INVALID        = -1008,  //һ   ״    ݵ λ   쳣
  ERR_RECEIVE_BUFFER_SMALL          = -1009,  //     buffer      С̫С

  ///////////2023-06-02
  ERR_SHARK_MOTOR_BLOCKED           = -1515,//motor blocked

  // ״  ڲ       
  ERR_LIDAR_FPS_INVALID    = -3001,  // fps 쳣
  ERR_LIDAR_SPEED_LOW      = -3002,  // ٶȵ   
  ERR_LIDAR_SPEED_HIGH     = -3003,  // ٶȸ   
  ERR_LIDAR_NUMBER_INVALID = -3004,  //  Ч     쳣
  ERR_LIDAR_SAME_ANGLE     = -3005,  //       ͬһ Ƕ 

};

class ReadParsePackage {
public:
  ReadParsePackage();
  ~ReadParsePackage();
  ///ֱ ӽ      ݵĽӿ 
  ///   裺 򿪴  ڣ   ȡ   ݣ        ݣ  رմ   
  int OpenSerial(char* port, unsigned int baud);
  void CloseSerial();
  int ReadDataSerial(unsigned int TimeOut_ms = 60);
  int ParseDataSerial(rangedata* dataPack, int& fps);
  ///--------------------ֱ ӽ      ݵĽӿ 

  void SetStartTimestamp() {
    startTimeStamp_ = GetTimeStamp();
  }
  //  ȡ      
  int GetLastErrCode() {
    std::unique_lock<std::mutex> lkr(mut);
    int err = m_lastErrorCode;

    if (m_lastErrorCode != 0)
      m_lastErrorCode = 0;
    return err;
  }

  void SetLastErrCode(int err) {
    std::unique_lock<std::mutex> lkr(mut);
    m_lastErrorCode = err;
  }

  int data_num_per_pack_;

private:
  int64_t GetTimeStamp();
  bool CheckLidarData(BYTE* buffer, int len);
  void CheckInvalidFPS(int validNumber);

  
  void CheckInvalidLowSpeed(unsigned int speed);
  void CheckInvalidHighSpeed(unsigned int speed);

  int GetDataByteLength(char cInfo);
  void SetReadCharsError(int errCode);

  int ParseLidarData(BYTE* in_buffer, const int in_numData, rangedata* out_dataPack);

  int ReadSurplusData(unsigned int TimeOut_ms);
  int ReadLidarData(unsigned int TimeOut_ms, BYTE cInfo);

  BYTE rcvbuffer_[PACKAGESIZE];

  int unitDataSize_;

  int data_num_total_;
  int data_rate_hz_;
  // TimeOut data_timer_;
  int64_t startTimeStamp_;
  rOc_serial serial_;

  std::atomic<int> m_lastErrorCode;

  int invalidFPSSecond_ = 0;

  int64_t startTimeLowSpeed_  = 0;
  int64_t startTimeHighSpeed_ = 0;

  std::mutex mut;

  ///////////2023-06-02
  void checkBlockSpeed(unsigned int u16speed);
  int64_t m_u64checkBlockSpeed = 0;
};

#endif