/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2012 The MITRE Corporation                                      *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <openbr/plugins/openbr_internal.h>
#include <openbr/core/opencvutils.h>

using namespace cv;

namespace br
{

/*!
 * \ingroup transforms
 * \brief Crops the rectangular regions of interest.
 * \author Josh Klontz \cite jklontz
 */
class ROITransform : public UntrainableTransform
{
    Q_OBJECT
    Q_PROPERTY(QString propName READ get_propName WRITE set_propName RESET reset_propName STORED false)
    BR_PROPERTY(QString, propName, "")

    void project(const Template &src, Template &dst) const
    {
        if (!propName.isEmpty()) {
            QRectF rect = src.file.get<QRectF>(propName);
            dst += src.m()(OpenCVUtils::toRect(rect));
        } else if (!src.file.rects().empty()) {
            foreach (const QRectF &rect, src.file.rects())
                dst += src.m()(OpenCVUtils::toRect(rect));
        } else if (src.file.contains(QStringList() << "X" << "Y" << "Width" << "Height")) {
            dst += src.m()(Rect(src.file.get<int>("X"),
                                src.file.get<int>("Y"),
                                src.file.get<int>("Width"),
                                src.file.get<int>("Height")));
        } else {
            dst = src;
            if (Globals->verbose)
                qWarning("No rects present in file.");
        }
        dst.file.clearRects();
    }
};

BR_REGISTER(Transform, ROITransform)

} // namespace br

#include "imgproc/roi.moc"
