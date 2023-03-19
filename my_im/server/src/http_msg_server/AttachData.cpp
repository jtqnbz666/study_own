#include "AttachData.h"

CDbAttachData::CDbAttachData(uint32_t type, uint32_t handle, uint32_t service_type) {
    CByteStream os(&m_buf, 0);

    os << type;
    os << handle;
    os << service_type;
    wstring wstrIn;
}

CDbAttachData::CDbAttachData(uchar_t* attach_data, uint32_t attach_len) {
    CByteStream is(attach_data, attach_len);

    is >> m_type;
    is >> m_handle;
    is >> m_service_type;
}

CPduAttachData::CPduAttachData(uint32_t type, uint32_t handle, uint32_t pduLength, uchar_t* pdu, uint32_t service_type) {
    CByteStream os(&m_buf, 0);

    os << type;
    os << handle;
    os << service_type;
    os.WriteData(pdu, pduLength); //先写长度pduLength， 再写pdu
}

CPduAttachData::CPduAttachData(uchar_t* attach_data, uint32_t attach_len) {
    CByteStream is(attach_data, attach_len);

    is >> m_type;
    is >> m_handle;
    is >> m_service_type;
    m_pdu = is.ReadData(m_pduLength); //先把长度读出来， 再读取后边接着的数据(pdu)
}