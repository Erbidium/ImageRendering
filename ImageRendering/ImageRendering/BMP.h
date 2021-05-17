#pragma pack(push, 1)
struct BMP {
    int8_t id[2];            // ������ �� ����� 'B' � 'M'
    int32_t filesize;        // ����� ����� � ������
    int32_t reserved;        // 0, 0
    int32_t headersize;      // 54L ��� 24-����� ���������
    int32_t infoSize;        // 40L ��� 24-����� ���������
    int32_t width;           // ������ ���������� � �������
    int32_t depth;           // ������ ���������� � �������
    int16_t biPlanes;        // 1 (��� 24-����� ���������)
    int16_t bits;            // 24 (��� 24-����� ���������)
    int32_t biCompression;   // 0L
    int32_t biSizeImage;     // ����� ��������� � 0L ��� ��������� ��� �������� (��� ������)
    int32_t biXPelsPerMeter; // ������������� ������� ������ �� ����, ����� 0L
    int32_t biYPelsPerMeter; // �� ����, �� �����
    int32_t biClrUsed;       // ��� ������������ ���������, ����� ��������� 0L
    int32_t biClrImportant;  // �� ����
    BMP() {
        id[0] = 'B';
        id[1] = 'M';
        reserved = 0;
        infoSize = 40;
        biPlanes = 1;
        bits = 24;
        biCompression = 0;
        biYPelsPerMeter = 2834;
        biClrUsed = 0;
        biClrImportant = 0;
    }
};
#pragma pack(pop)