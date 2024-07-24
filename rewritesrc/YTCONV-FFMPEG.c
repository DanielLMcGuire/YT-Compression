#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>

void logo() {
    const char *VERSION = "0.1.0.7.2430";
    const char *TITLE = "YouTube Compression Tool";
    char LOGO[256];
    snprintf(LOGO, sizeof(LOGO), "%s %s", TITLE, VERSION);
    printf("\033[34m%s\033[0m\n", LOGO); // Blue text
}

void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, AVFormatContext *output_ctx) {
    int ret;

    // Send the frame to the encoder
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    // Receive the encoded packet
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        // Write the encoded packet to the output file
        pkt->stream_index = 0;
        av_interleaved_write_frame(output_ctx, pkt);
        av_packet_unref(pkt);
    }
}

int main(int argc, char **argv) {
     logo();
    AVFormatContext *input_ctx = NULL, *output_ctx = NULL;
    AVCodecContext *dec_ctx, *enc_ctx;
    const AVCodec *dec, *enc;
    AVStream *in_stream, *out_stream;
    AVPacket *pkt;
    AVFrame *frame;
    int ret;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    // Initialize libavformat and register all the formats and codecs
    avformat_network_init();

    // Open input file
    if (avformat_open_input(&input_ctx, argv[1], NULL, NULL) < 0) {
        fprintf(stderr, "Could not open input file '%s'\n", argv[1]);
        return 1;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(input_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        return 1;
    }

    // Open output file
    avformat_alloc_output_context2(&output_ctx, NULL, NULL, argv[2]);
    if (!output_ctx) {
        fprintf(stderr, "Could not create output context\n");
        return 1;
    }

    // Find the video stream in the input file
    int video_stream_index = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (video_stream_index < 0) {
        fprintf(stderr, "Could not find video stream in input file\n");
        return 1;
    }

    // Get the decoder context
    in_stream = input_ctx->streams[video_stream_index];
    dec_ctx = avcodec_alloc_context3(dec);
    avcodec_parameters_to_context(dec_ctx, in_stream->codecpar);
    avcodec_open2(dec_ctx, dec, NULL);

    // Find the encoder
    enc = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!enc) {
        fprintf(stderr, "Necessary encoder not found\n");
        return 1;
    }

    // Allocate the encoder context
    enc_ctx = avcodec_alloc_context3(enc);
    enc_ctx->height = dec_ctx->height;
    enc_ctx->width = dec_ctx->width;
    enc_ctx->sample_aspect_ratio = dec_ctx->sample_aspect_ratio;
    enc_ctx->pix_fmt = enc->pix_fmts[0];
    enc_ctx->time_base = av_inv_q(dec_ctx->framerate);

    // Open the encoder
    if (avcodec_open2(enc_ctx, enc, NULL) < 0) {
        fprintf(stderr, "Could not open encoder\n");
        return 1;
    }

    // Create the new stream for the output file
    out_stream = avformat_new_stream(output_ctx, NULL);
    if (!out_stream) {
        fprintf(stderr, "Failed allocating output stream\n");
        return 1;
    }
    avcodec_parameters_from_context(out_stream->codecpar, enc_ctx);

    // Open the output file
    if (!(output_ctx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_ctx->pb, argv[2], AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open output file '%s'\n", argv[2]);
            return 1;
        }
    }

    // Write the stream header
    if (avformat_write_header(output_ctx, NULL) < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        return 1;
    }

    // Allocate packet and frame
    pkt = av_packet_alloc();
    frame = av_frame_alloc();
    frame->format = enc_ctx->pix_fmt;
    frame->width = enc_ctx->width;
    frame->height = enc_ctx->height;
    av_frame_get_buffer(frame, 0);

    // Read frames from the input file
    while (av_read_frame(input_ctx, pkt) >= 0) {
        if (pkt->stream_index == video_stream_index) {
            // Send the packet to the decoder
            ret = avcodec_send_packet(dec_ctx, pkt);
            if (ret < 0) {
                fprintf(stderr, "Error sending a packet for decoding\n");
                break;
            }

            // Receive the frame from the decoder
            ret = avcodec_receive_frame(dec_ctx, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                continue;
            else if (ret < 0) {
                fprintf(stderr, "Error during decoding\n");
                break;
            }

            // Send the frame to the encoder
            encode(enc_ctx, frame, pkt, output_ctx);
        }
        av_packet_unref(pkt);
    }

    // Flush the encoder
    encode(enc_ctx, NULL, pkt, output_ctx);

    // Write the trailer
    av_write_trailer(output_ctx);

    // Clean up
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avcodec_free_context(&enc_ctx);
    avcodec_free_context(&dec_ctx);
    avformat_close_input(&input_ctx);
    if (output_ctx && !(output_ctx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&output_ctx->pb);
    avformat_free_context(output_ctx);

    return 0;
}
