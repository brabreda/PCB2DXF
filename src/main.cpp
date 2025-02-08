#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/core/fast_math.hpp>

#include <algorithm>
#include <ranges>
#include <array>



class EdgeOnlyCharucoBoard {

    public:

    EdgeOnlyCharucoBoard(   const cv::Size& size, float squareLength, float markerLength,
                            const cv::aruco::Dictionary &dictionary, cv::InputArray ids = cv::noArray()) 
        : m_Dictionary(dictionary)
        , m_Size(size)
        , m_Board(size, squareLength, markerLength, dictionary, ids)

    {
        std::cout   << " Size: " <<  m_Board.getObjPoints().size()  
                    << " size[0]: " << m_Board.getObjPoints()[0].size()
                    << " size[1]: " << m_Board.getObjPoints()[1].size() << std::endl;
    }
    /*  Functionlality of this function is based on the cv::aruco::Board::generateImage, it is reproduced as close as 
    *   possible. This way the white center erea is accurate and does not cover any part of the squares from the sides.
    *   - TODO: check that we don't overwrite a part of the edge squares.
    */
    void generateImage(cv::Size outSize, cv::OutputArray img, int marginSize = 0, int borderBits = 1) const {
        m_Board.generateImage(outSize, img, marginSize, borderBits);
        
        cv::Size size = m_Board.getChessboardSize();
        
        cv::Mat out = img.getMat();
        cv::Mat noMarginsImg = out.colRange(marginSize, out.cols - marginSize).rowRange(marginSize, out.rows - marginSize);

        const float pixInSquareX = (float)noMarginsImg.cols / (float)size.width;
        const float pixInSquareY = (float)noMarginsImg.rows / (float)size.height;
        const float pixInSquare = std::min(pixInSquareX, pixInSquareY);
        
        cv::Size pixInChessboard(noMarginsImg.cols, noMarginsImg.rows);
        cv::Point startChessboard(0, 0);
        if (pixInSquareX <= pixInSquareY) {
            pixInChessboard.height = cvRound(pixInSquare*size.height);
            int rowsMargin = (noMarginsImg.rows - pixInChessboard.height) / 2;
            startChessboard.y = rowsMargin;
        }
        else {
            pixInChessboard.width = cvRound(pixInSquare*size.width);
            int colsMargin = (noMarginsImg.cols - pixInChessboard.width) / 2;
            startChessboard.x = colsMargin;
        }

        cv::Mat chessboardZoneImg = noMarginsImg(cv::Rect(startChessboard, pixInChessboard));

        cv::Mat squareZone = chessboardZoneImg(cv::Range(cvRound(pixInSquare), cvRound(chessboardZoneImg.rows - pixInSquare)),
                                               cv::Range(cvRound(pixInSquare), cvRound(chessboardZoneImg.cols - pixInSquare)));

        squareZone.setTo(255);
    };

    private:

    cv::aruco::Dictionary m_Dictionary;
    cv::Size m_Size;
    cv::aruco::CharucoBoard m_Board;
};

struct A4Size : public cv::Size { A4Size() : cv::Size(210,297) {} };

int main() {
    float squareLength = 4.0;  // Size of each square (in mm or pixels)
    float markerLength = 3.0;  // Size of each marker (in mm or pixels)
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);

    
    cv::Mat img;
    
    EdgeOnlyCharucoBoard board(cv::Size(9,13), squareLength, markerLength, dictionary);
    board.generateImage(A4Size(), img, 10, 1);
    
    cv::imwrite("charuco_board.png", img);
    
    return 0;
}