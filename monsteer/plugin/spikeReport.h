
/* Copyright (c) 2006-2015, Juan Hernando <jhernando@fi.upm.es>
 *                          Ahmet Bilgili <ahmet.bilgili@epfl.ch>
 *
 * This file is part of Monsteer <https://github.com/BlueBrain/Monsteer>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MONSTEER_PLUGIN_SPIKEREPORT_H
#define MONSTEER_PLUGIN_SPIKEREPORT_H

#include <monsteer/types.h>
#include <zeroeq/types.h>

#include <monsteer/plugin/endOfStream.h>
#include <monsteer/plugin/spikes.h>

#include <brion/spikeReportPlugin.h>

#include <boost/scoped_ptr.hpp>

namespace monsteer
{
namespace plugin
{

/** A ZeroEQ streaming spike report reader/writer. Class is not thread safe. */
class SpikeReport : public brion::SpikeReportPlugin
{
public:
    /** Create a new streaming NEST report. */
    explicit SpikeReport( const brion::SpikeReportInitData& pluginData );

    /** Check if this plugin can handle the given plugin data. */
    static bool handles( const brion::SpikeReportInitData& pluginData );

    float getStartTime() const final;

    float getEndTime() const final;

    void writeSpikes( const brion::Spikes& spikes ) final;

    const brion::Spikes& getSpikes() const final;

    bool waitUntil( float timeStamp, uint32_t timeout ) final;

    float getNextSpikeTime() final;

    float getLatestSpikeTime() final;

    void clear( float startTime, float endTime );

    brion::SpikeReport::ReadMode getReadMode() const final;

    const URI& getURI() const final;

    void close() final;

private:
    const URI _uri;
    brion::Spikes _incoming;
    brion::Spikes _spikes;

    float _lastEndTime;
    float _lastTimeStamp;

    boost::scoped_ptr< zeroeq::Subscriber > _subscriber;
    boost::scoped_ptr< zeroeq::Publisher > _publisher;

    bool _closed;

    void _onSpikes( ConstSpikesEventPtr event );
    void _onEOS();

    void _receiveBufferedMessages();
};

}
}
#endif
