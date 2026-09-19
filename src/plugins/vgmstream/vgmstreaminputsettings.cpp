/*
 * VGMStream Plugin
 * Copyright © 2025, Christopher Snowhill <kode54@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "vgmstreaminputsettings.h"

#include "vgmstreaminputdefs.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSpinBox>

using namespace Qt::StringLiterals;

namespace Fooyin::VGMStreamInput {
VGMStreamInputSettings::VGMStreamInputSettings(QWidget* parent)
    : QDialog{parent}
    , m_loopNormally{new QRadioButton(tr("Loop normally"), this)}
    , m_loopForever{new QRadioButton(tr("Loop forever"), this)}
    , m_ignoreLoop{new QRadioButton(tr("Ignore loop points"), this)}
    , m_loopCount{new QDoubleSpinBox(this)}
    , m_fadeLength{new QSpinBox(this)}
    , m_fadeDelay{new QSpinBox(this)}
    , m_downmixChannels{new QSpinBox(this)}
    , m_disableSubsongs{new QCheckBox(tr("Disable subsongs"), this)}
    , m_disableTagfile{new QCheckBox(tr("Disable %1 tags").arg(u"!tags.m3u"_s), this)}
    , m_commonExts{new QCheckBox(tr("Play common file types"), this)}
{
    setWindowTitle(tr("%1 Settings").arg(u"VGMStream Input"_s));
    setModal(true);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QObject::connect(buttons, &QDialogButtonBox::accepted, this, &VGMStreamInputSettings::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, this, &VGMStreamInputSettings::reject);

    auto* loopGroup  = new QGroupBox(tr("Looping"), this);
    auto* loopLayout = new QGridLayout(loopGroup);

    auto* loopCountLabel = new QLabel(tr("Loop count") + u":"_s, this);

    m_loopCount->setRange(0.5, 10.0);
    m_loopCount->setSingleStep(0.5);
    m_loopCount->setDecimals(2);
    m_loopCount->setSuffix(u" "_s + tr("loops"));

    int row{0};
    loopLayout->addWidget(m_loopNormally, row++, 0, 1, 2);
    loopLayout->addWidget(m_loopForever, row++, 0, 1, 2);
    loopLayout->addWidget(m_ignoreLoop, row++, 0, 1, 2);
    loopLayout->addWidget(loopCountLabel, row, 0);
    loopLayout->addWidget(m_loopCount, row++, 1);
    loopLayout->setColumnStretch(2, 1);

    auto* lengthGroup  = new QGroupBox(tr("Fade"), this);
    auto* lengthLayout = new QGridLayout(lengthGroup);

    auto* fadeLabel = new QLabel(tr("Fade length") + u":"_s, this);

    m_fadeLength->setRange(0, 60000);
    m_fadeLength->setSingleStep(500);
    m_fadeLength->setSuffix(u" "_s + tr("ms"));

    auto* fadeDelayLabel = new QLabel(tr("Fade delay") + u":"_s, this);

    m_fadeDelay->setRange(0, 60000);
    m_fadeDelay->setSingleStep(500);
    m_fadeDelay->setSuffix(u" "_s + tr("ms"));

    row = 0;
    lengthLayout->addWidget(fadeLabel, row, 0);
    lengthLayout->addWidget(m_fadeLength, row++, 1);
    lengthLayout->addWidget(fadeDelayLabel, row, 0);
    lengthLayout->addWidget(m_fadeDelay, row++, 1);
    lengthLayout->setColumnStretch(2, 1);

    auto* generalGroup  = new QGroupBox(tr("General"), this);
    auto* generalLayout = new QGridLayout(generalGroup);

    auto* downmixLabel = new QLabel(tr("Downmix channels") + u":"_s, this);

    m_downmixChannels->setRange(0, 8);
    m_downmixChannels->setSingleStep(1);
    m_downmixChannels->setSpecialValueText(tr("Disabled"));
    m_downmixChannels->setToolTip(tr("Downmix streams with more channels than this. Very simplistic; not recommended."));

    m_commonExts->setToolTip(
        tr("Also claim common file types such as %1. May conflict with other decoders.").arg(u"mp3, ogg, wav"_s));

    row = 0;
    generalLayout->addWidget(downmixLabel, row, 0);
    generalLayout->addWidget(m_downmixChannels, row++, 1);
    generalLayout->addWidget(m_disableSubsongs, row++, 0, 1, 2);
    generalLayout->addWidget(m_disableTagfile, row++, 0, 1, 2);
    generalLayout->addWidget(m_commonExts, row++, 0, 1, 2);
    generalLayout->setColumnStretch(2, 1);

    auto* layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    row = 0;
    layout->addWidget(loopGroup, row++, 0, 1, 4);
    layout->addWidget(lengthGroup, row++, 0, 1, 4);
    layout->addWidget(generalGroup, row++, 0, 1, 4);
    layout->addWidget(buttons, row++, 0, 1, 4, Qt::AlignBottom);
    layout->setColumnStretch(2, 1);

    switch(static_cast<LoopMode>(m_settings.value(LoopModeKey, DefaultLoopMode).toInt())) {
        case LoopMode::Forever:
            m_loopForever->setChecked(true);
            break;
        case LoopMode::Ignore:
            m_ignoreLoop->setChecked(true);
            break;
        case LoopMode::Normal:
        default:
            m_loopNormally->setChecked(true);
            break;
    }

    m_loopCount->setValue(m_settings.value(LoopCount, DefaultLoopCount).toDouble());
    m_fadeLength->setValue(m_settings.value(FadeLength, DefaultFadeLength).toInt());
    m_fadeDelay->setValue(m_settings.value(FadeDelay, DefaultFadeDelay).toInt());
    m_downmixChannels->setValue(m_settings.value(DownmixChannels, DefaultDownmixChannels).toInt());
    m_disableSubsongs->setChecked(m_settings.value(DisableSubsongs, DefaultDisableSubsongs).toBool());
    m_disableTagfile->setChecked(m_settings.value(DisableTagfile, DefaultDisableTagfile).toBool());
    m_commonExts->setChecked(m_settings.value(CommonExts, DefaultCommonExts).toBool());
}

void VGMStreamInputSettings::accept()
{
    auto mode = LoopMode::Normal;
    if(m_loopForever->isChecked()) {
        mode = LoopMode::Forever;
    } else if(m_ignoreLoop->isChecked()) {
        mode = LoopMode::Ignore;
    }

    m_settings.setValue(LoopModeKey, static_cast<int>(mode));
    m_settings.setValue(LoopCount, m_loopCount->value());
    m_settings.setValue(FadeLength, m_fadeLength->value());
    m_settings.setValue(FadeDelay, m_fadeDelay->value());
    m_settings.setValue(DownmixChannels, m_downmixChannels->value());
    m_settings.setValue(DisableSubsongs, m_disableSubsongs->isChecked());
    m_settings.setValue(DisableTagfile, m_disableTagfile->isChecked());
    m_settings.setValue(CommonExts, m_commonExts->isChecked());

    done(Accepted);
}

} // namespace Fooyin::VGMStreamInput
