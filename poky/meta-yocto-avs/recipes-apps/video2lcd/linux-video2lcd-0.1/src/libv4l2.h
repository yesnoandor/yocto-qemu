#ifndef _LIBV4L2_H_
#define _LIBV4L2_H_

#include <linux/videodev2.h>
#include <stdint.h>



struct v4l2_buf_unit {
    int                index;
    void*              start;
    uint32_t           length;
    uint32_t           offset;
};

struct v4l2_buf {
    struct v4l2_buf_unit* buf;
    int nr_bufs;
    enum v4l2_buf_type type;
};



/*
 * �������ƣ�v4l2_open
 * ������������v4l2�豸
 * ���������name - �豸����
 * ���������flag - ���豸��־����open��ͬ
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ������ļ�������
 */ 
int v4l2_open(const char* name, int flag);



/*
 * �������ƣ�v4l2_close
 * �����������ر�v4l2�豸
 * ���������fd - v4l2�豸���ļ�������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */ 
int v4l2_close(int fd);



/*
 * �������ƣ�v4l2_querycap
 * ������������ѯv4l2�豸����
 * ���������fd   - v4l2�豸���ļ�������
 * ���������cap  - �õ����豸��Ϣ
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_querycap(int fd, struct v4l2_capability* cap);



/*
 * �������ƣ�v4l2_enuminput
 * ����������ö�������豸
 * ���������fd         - v4l2�豸���ļ�������
 * ���������index - �����豸������
 * ���������name       - ���������豸������(�˺���������һ��buf)
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_enuminput(int fd, int index, char* name);



/*
 * �������ƣ�v4l2_s_input
 * �������������������豸
 * ���������fd         - v4l2�豸���ļ�������
 * ���������index - �����豸������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_s_input(int fd, int index);



/*
 * �������ƣ�v4l2_enum_fmt
 * ����������ö���豸֧�ֵĸ�ʽ���ж��Ƿ�֧��ָ���ĸ�ʽ
 * ���������fd         - v4l2�豸���ļ�������
 * ���������fmt        - ָ��ͼ���ʽ
 * �����������
 * �� �� ֵ����֧�ַ���-1;֧�ַ���0
 *
 * fmt��
 *  V4L2_PIX_FMT_RGB565
 *  V4L2_PIX_FMT_RGB32
 *  V4L2_PIX_FMT_YUYV
 *  V4L2_PIX_FMT_UYVY
 *  V4L2_PIX_FMT_VYUY
 *  V4L2_PIX_FMT_YVYU
 *  V4L2_PIX_FMT_YUV422P
 *  V4L2_PIX_FMT_NV12
 *  V4L2_PIX_FMT_NV12T
 *  V4L2_PIX_FMT_NV21
 *  V4L2_PIX_FMT_NV16
 *  V4L2_PIX_FMT_NV61
 *  V4L2_PIX_FMT_YUV420
 *  V4L2_PIX_FMT_JPEG
 */
int v4l2_enum_fmt(int fd, unsigned int fmt, enum v4l2_buf_type type);



/*
 * �������ƣ�v4l2_s_fmt
 * ��������������ͼ���ʽ
 * ���������fd          - v4l2�豸���ļ�������
 * ���������width  - ͼ����
 * ���������height - ͼ��߶�
 * ���������fmt         - ���ظ�ʽ
 * ���������type   - ��������
 * ���������width  - �޸Ĺ����ͼ����
 * ���������height - �޸Ĺ����ͼ��߶�
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 *
 * fmt��
 *  V4L2_PIX_FMT_RGB565
 *  V4L2_PIX_FMT_RGB32
 *  V4L2_PIX_FMT_YUYV
 *  V4L2_PIX_FMT_UYVY
 *  V4L2_PIX_FMT_VYUY
 *  V4L2_PIX_FMT_YVYU
 *  V4L2_PIX_FMT_YUV422P
 *  V4L2_PIX_FMT_NV12
 *  V4L2_PIX_FMT_NV12T
 *  V4L2_PIX_FMT_NV21
 *  V4L2_PIX_FMT_NV16
 *  V4L2_PIX_FMT_NV61
 *  V4L2_PIX_FMT_YUV420
 *  V4L2_PIX_FMT_JPEG
 *
 * type��
 *  V4L2_BUF_TYPE_VIDEO_CAPTURE
 *  V4L2_BUF_TYPE_VIDEO_OUTPUT
 *  V4L2_BUF_TYPE_VIDEO_OVERLAY
 *  V4L2_BUF_TYPE_VBI_CAPTURE
 *  V4L2_BUF_TYPE_VBI_OUTPUT
 *  V4L2_BUF_TYPE_SLICED_VBI_CAPTURE
 *  V4L2_BUF_TYPE_SLICED_VBI_OUTPUT
 *  V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY
 *  V4L2_BUF_TYPE_PRIVATE
 */
int v4l2_s_fmt(int fd, int* width, int* height, unsigned int fmt, enum v4l2_buf_type type);



/*
 * �������ƣ�v4l2_reqbufs
 * �������������뻺��
 * ���������fd            - v4l2�豸���ļ�������
 * ���������tyep          - ��������
 * ���������nr_bufs       - ��������
 * �����������
 * �� �� ֵ��ʧ�ܷ���NULL;�ɹ�����v4l2_buf�ṹ��ָ��
 */
struct v4l2_buf* v4l2_reqbufs(int fd, enum v4l2_buf_type type, int nr_bufs);



/*
 * �������ƣ�v4l2_querybuf
 * ������������ѯ������Ϣ����
 * ���������fd            - v4l2�豸���ļ�������
 * ���������v4l2_buf      - v4l2_buf�ṹ��ָ��
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_querybuf(int fd, struct v4l2_buf* v4l2_buf);



/*
 * �������ƣ�v4l2_mmap
 * ����������ӳ�仺��
 * ���������fd            - v4l2�豸���ļ�������
 * ���������v4l2_buf      - v4l2_buf�ṹ��ָ��
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_mmap(int fd, struct v4l2_buf* v4l2_buf);



/*
 * �������ƣ�v4l2_munmap
 * ����������ȡ������ӳ��
 * ���������fd            - v4l2�豸���ļ�������
 * ���������v4l2_buf      - v4l2_buf�ṹ��ָ��
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_munmap(int fd, struct v4l2_buf* v4l2_buf);



/*
 * �������ƣ�v4l2_relbufs
 * �����������ͷŻ���
 * ���������v4l2_buf      - v4l2_buf�ṹ��ָ��
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_relbufs(struct v4l2_buf* v4l2_buf);



/*
 * �������ƣ�v4l2_streamon
 * ������������ʼ�ɼ�
 * ���������fd      - v4l2�ļ�������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_streamon(int fd);



/*
 * �������ƣ�v4l2_streamon
 * ����������ֹͣ�ɼ�
 * ���������fd      - v4l2�ļ�������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_streamoff(int fd);



/*
 * �������ƣ�v4l2_qbuf
 * �������������������
 * ���������fd     -v4l2�ļ�������
 * ���������buf-���浥Ԫ
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_qbuf(int fd, struct v4l2_buf_unit* buf);



/*
 * �������ƣ�v4l2_qbuf_all
 * �������������л��������
 * ���������fd     -v4l2�ļ�������
 * ���������v4l2_buf     -����
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_qbuf_all(int fd, struct v4l2_buf* v4l2_buf);



/*
 * �������ƣ�v4l2_dqbuf
 * �������������������
 * ���������fd     -v4l2�ļ�������
 * �����������
 * �� �� ֵ��ʧ�ܷ���NULL;�ɹ�����buf��Ԫ
 */
struct v4l2_buf_unit* v4l2_dqbuf(int fd, struct v4l2_buf* v4l2_buf);




/*
 * �������ƣ�v4l2_g_ctrl
 * ������������ȡָ��control��Ϣ
 * ���������fd      - v4l2�ļ�������
 * ���������id - control id
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����control id��Ӧ��value
 */
int v4l2_g_ctrl(int fd, unsigned int id);



/*
 * �������ƣ�v4l2_s_ctrl
 * �������������������
 * ���������fd         - v4l2�ļ�������
 * ���������id         - control id
 * ���������value      - control id ��Ӧ��value
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_s_ctrl(int fd, unsigned int id, unsigned int value);



/*
 * �������ƣ�v4l2_g_parm
 * ������������ȡ����
 * ���������fd              - v4l2�ļ�������
 * ���������streamparm - ��ȡ���Ĳ���
 * �� �� ֵ��NULL-ʧ��;��NULL - v4l2_buf_unit�ṹ��ָ��
 */
int v4l2_g_parm(int fd, struct v4l2_streamparm* streamparm);



/*
 * �������ƣ�v4l2_s_parm
 * �������������ò���
 * ���������fd              - v4l2�ļ�������
 * ���������streamparm - ������������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_s_parm(int fd, struct v4l2_streamparm *streamparm);



/*
 * �������ƣ�v4l2_poll
 * �����������ȴ��������
 * ���������fd      - v4l2�ļ�������
 * �����������
 * �� �� ֵ��ʧ�ܷ���-1;�ɹ�����0
 */
int v4l2_poll(int fd);



#endif //_LIBV4L2_H_
